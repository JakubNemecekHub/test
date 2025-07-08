from pathlib import Path
import re


TEST_FOLDER: str = ".\\test\\src"


def check_test_src_folder() -> bool:
    TEST_FOLDER_PATH: Path = Path(TEST_FOLDER)
    return TEST_FOLDER_PATH.exists()


def main() -> None:
    if not check_test_src_folder():
        return

    SOURCE_FILE_PATH: Path = Path(TEST_FOLDER).joinpath("_tests.hpp")
    if not SOURCE_FILE_PATH.exists():
        print(f"file {SOURCE_FILE_PATH} doesn't exist.")
        return

    pattern: re.Pattern[str] = re.compile(r"^ts::Suite +(\w+) +{(.+)}")
    suites: list[str] = []
    with open(SOURCE_FILE_PATH, "r") as file:
        for line in file:
            res = pattern.search(line)
            if res:
                suites.append(res.group(1))

    if len(suites) == 0:
        print("No test suites found in _tests.hpp.")
        return
    suites = [f"\ttester.add({suite}, \"{suite}\");\n" for suite in suites]

    PART_1: list[str] = [
        "#include <windows.h>\n\n",
        "#include \"../lib/ts/tester.hpp\"\n",
        "#include \"_tests.hpp\"\n\n\n",
        "int main()\n"
        "{\n\n",
        "\tSetConsoleOutputCP(CP_UTF8);\n",
        "\tts::Tester tester;\n"
    ]
    PART_2: list[str] = [
        "\ttester.run();\n",
        "\treturn 0;\n\n",
        "}\n"
    ]

    MAIN_FILE_PATH: Path = Path(TEST_FOLDER).joinpath("test.cpp")
    with open(MAIN_FILE_PATH, "w") as file:
        file.writelines(PART_1)
        file.writelines(suites)
        file.writelines(PART_2)


if __name__ == "__main__":
    main()
