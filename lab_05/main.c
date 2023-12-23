#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256
#define ALPHABETS_NUM 256

int num_active = 0;
int *frequency = NULL;
unsigned int original_size = 0;

typedef struct {
  int index;
  unsigned int weight;
} node_t;

node_t *nodes = NULL;
int num_nodes = 0;
int *leaf_index = NULL;
int *parent_index = NULL;

int free_index = 1;

int *stack;
int stack_top;

unsigned char buffer[MAX_BUFFER_SIZE];
int bits_in_buffer = 0;
int current_bit = 0;

typedef enum { COMPRESS = 0, DECOMPRESS } Modes;

struct arguments {
  Modes mode;
};

void determine_frequency(FILE *f) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    ++frequency[c];
    ++original_size;
  }
  for (c = 0; c < ALPHABETS_NUM; ++c) {
    if (frequency[c] > 0) {
      ++num_active;
    }
  }
}

void init() {
  frequency = (int *)calloc(2 * ALPHABETS_NUM, sizeof(int));
  leaf_index = frequency + ALPHABETS_NUM - 1;
}

void allocate_tree() {
  nodes = (node_t *)calloc(2 * num_active, sizeof(node_t));
  parent_index = (int *)calloc(num_active, sizeof(int));
}

int add_node(int index, int weight) {
  int i = num_nodes++;
  while (i > 0 && nodes[i].weight > weight) {
    memcpy(&nodes[i + 1], &nodes[i], sizeof(node_t));
    if (nodes[i].index < 0) {
      ++leaf_index[-nodes[i].index];
    } else {
      ++parent_index[nodes[i].index];
    }
    --i;
  }

  ++i;
  nodes[i].index = index;
  nodes[i].weight = weight;
  if (index < 0) {
    leaf_index[-index] = i;
  } else {
    parent_index[index] = i;
  }

  return i;
}

void add_leaves() {
  int i, freq;
  for (i = 0; i < ALPHABETS_NUM; ++i) {
    freq = frequency[i];
    if (freq > 0) {
      add_node(-(i + 1), freq);
    }
  }
}

void build_tree() {
  int a, b, index;
  while (free_index < num_nodes) {
    a = free_index++;
    b = free_index++;
    index = add_node(b / 2, nodes[a].weight + nodes[b].weight);
    parent_index[b / 2] = index;
  }
}

void write_header(FILE *f) {
  int size = sizeof(unsigned int) + 1 + num_active * (1 + sizeof(int));
  char *buffer = (char *)calloc(size, 1);
  if (buffer == NULL) {
    return;
  }

  int j = sizeof(int);
  int byte = 0;
  while (j--) {
    buffer[byte++] = (original_size >> (j << 3)) & 0xff;
  }
  buffer[byte++] = (char)num_active;
  for (int i = 1; i <= num_active; ++i) {
    unsigned int weight = nodes[i].weight;
    buffer[byte++] = (char)(-nodes[i].index - 1);
    j = sizeof(int);
    while (j--) {
      buffer[byte++] = (weight >> (j << 3)) & 0xff;
    }
  }

  fwrite(buffer, 1, size, f);
  free(buffer);
}

int read_header(FILE *f) {
  unsigned char buff[4];

  size_t bytes_read = fread(&buff, 1, sizeof(int), f);
  if (bytes_read < 1) {
    return -1;
  }

  int byte = 0;
  original_size = buff[byte++];
  while (byte < sizeof(int)) {
    original_size = (original_size << (1 << 3)) | buff[byte++];
  }

  bytes_read = fread(&num_active, 1, 1, f);
  if (bytes_read < 1) {
    return -1;
  }

  allocate_tree();

  int size = num_active * (1 + sizeof(int));
  char *buffer = (char *)calloc(size, 1);
  if (buffer == NULL) {
    return -1;
  }

  fread(buffer, 1, size, f);
  byte = 0;
  for (int i = 1; i <= num_active; ++i) {
    nodes[i].index = -(buffer[byte++] + 1);
    int j = 0;
    unsigned int weight = (unsigned char)buffer[byte++];
    while (++j < sizeof(int)) {
      weight = (weight << (1 << 3)) | (unsigned char)buffer[byte++];
    }
    nodes[i].weight = weight;
  }
  num_nodes = (int)num_active;

  free(buffer);
  return 0;
}

void write_bit(FILE *f, int bit) {
  if (bits_in_buffer == MAX_BUFFER_SIZE << 3) {
    fwrite(buffer, 1, MAX_BUFFER_SIZE, f);
    bits_in_buffer = 0;
    memset(buffer, 0, MAX_BUFFER_SIZE);
  }
  if (bit) {
    buffer[bits_in_buffer >> 3] |= (0x1 << (7 - bits_in_buffer % 8));
  }
  ++bits_in_buffer;
}

void flush_buffer(FILE *f) {
  if (bits_in_buffer) {
    fwrite(buffer, 1, (bits_in_buffer + 7) >> 3, f);
    bits_in_buffer = 0;
  }
}

int read_bit(FILE *f) {
  if (current_bit == bits_in_buffer) {
    size_t bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, f);
    bits_in_buffer = bytes_read << 3;
    current_bit = 0;
  }

  if (bits_in_buffer == 0) {
    return -1;
  }
  int bit = (buffer[current_bit >> 3] >> (7 - current_bit % 8)) & 0x1;
  ++current_bit;
  return bit;
}

void compress_alphabet(FILE *fout, int character) {
  int node_index;
  stack_top = 0;
  node_index = leaf_index[character + 1];
  while (node_index < num_nodes) {
    stack[stack_top++] = node_index % 2;
    node_index = parent_index[(node_index + 1) / 2];
  }
  while (--stack_top > -1) {
    write_bit(fout, stack[stack_top]);
  }
}

int compress(FILE *fin, FILE *fout) {
  determine_frequency(fin);
  stack = (int *)calloc(num_active - 1, sizeof(int));
  allocate_tree();

  add_leaves();
  write_header(fout);
  //printf("%d\n", original_size);
  build_tree();
  fseek(fin, 0, SEEK_SET);
  int c;
  while ((c = fgetc(fin)) != EOF) {
    compress_alphabet(fout, c);
  }
  flush_buffer(fout);
  free(stack);

  return 0;
}

void decompress_bit_stream(FILE *fin, FILE *fout) {
  int i = 0, bit, node_index = nodes[num_nodes].index;
  while (1) {
    bit = read_bit(fin);
    if (bit == -1) break;
    node_index = nodes[node_index * 2 - bit].index;
    if (node_index < 0) {
      char c = -node_index - 1;
      fwrite(&c, 1, 1, fout);
      if (++i == original_size) break;
      node_index = nodes[num_nodes].index;
    }
  }
}

int decompress(FILE *fin, FILE *fout) {
  if (read_header(fin) == 0) 
  {
    //printf("%d\n", original_size);
    build_tree();
    decompress_bit_stream(fin, fout);
  }
  return 0;
}

int main(const int argc, char **argv) {
  struct arguments arguments;
  if(argc != 4)
  {
    printf("Wrong argument!");
    return 0;
  }
  if (strcmp(argv[1], "c") == 0)
    arguments.mode = COMPRESS;
  else if (strcmp(argv[1], "d") == 0)
    arguments.mode = DECOMPRESS;
  else 
  {
    printf("Wrong mode!");
    return 0;
  }

  FILE *in_file = fopen(argv[2], "rb");
  if (!in_file) {
    return 1;
  }
  FILE *out_file = fopen(argv[3], "wb");
  if (!out_file) {
    fclose(in_file);
    return 1;
  }

  init();

  if (arguments.mode == COMPRESS) {
    compress(in_file, out_file);
  } else {
    decompress(in_file, out_file);
  }

  fclose(in_file);
  fclose(out_file);

  free(parent_index);
  free(frequency);
  free(nodes);

  return 0;
}