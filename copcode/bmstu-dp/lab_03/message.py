from dataclasses import dataclass


@dataclass
class Colors:
    END = '\033[0;0m'
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'


def danger_message(text: str) -> None:
    print(Colors.RED + text + Colors.END)

def success_message(text: str) -> None:
    print(Colors.GREEN + text + Colors.END)
