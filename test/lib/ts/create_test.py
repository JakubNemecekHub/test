import argparse
from pathlib import Path

TEST_FOLDER: str = ".\\test\\src"


def check_test_src_folder() -> bool:
    TEST_FOLDER_PATH: Path = Path(TEST_FOLDER)
    return TEST_FOLDER_PATH.exists()


def main(name: str, comment: str = "") -> None:
    if not check_test_src_folder():
        return

    # sanitize name

    # check if file exists
    TEST_FILE_PATH: Path = Path(TEST_FOLDER).joinpath(f"{name}.test.hpp")
    if TEST_FILE_PATH.exists():
        valid_input: bool = False
        while not valid_input:
            VALID_INPUTS: list[str] = ["Y", "y", "N", "n"]
            user_input = input(f"File {TEST_FILE_PATH} exists. Overwrite? [Y]es [N]o ")
            valid_input = user_input in VALID_INPUTS
            if user_input in VALID_INPUTS[2:]:
                return

    # create file {name}.test.hpp at TEST_FOLDER
    with open(TEST_FILE_PATH, "w") as file:
        file.write("#pragma once")
        file.write("\n")
        file.write("#include \"../lib/ts/suite.hpp\"")
        file.write("\n\n\n")
        if comment:
            suite: str = f"ts::Suite {name} {{ \"{comment}\" }};"
        else:
            suite: str = f"ts::Suite {name} {{}};"
        file.write(suite)


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("name", help="name of the test suite")
    parser.add_argument("-c", "--comment", help="optional comment for the test suite")

    args = parser.parse_args()

    main(args.name, args.comment)
