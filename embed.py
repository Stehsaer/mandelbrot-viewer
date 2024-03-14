import os
import sys


def embed_binary_as_source(binary_file_name, source_file_name, var_name):
    """
    Embeds a binary file as C++ source code.

    Args:
            binary_file_name (str): The path to the binary file.
            source_file_name (str): The path to the C++ source file to be generated.
            var_name (str): The name of the variable to store the binary data.

    Returns:
            None
    """
    # Read the binary file as a hexadecimal string

    binary_data = open(binary_file_name, "rb").read()

    # Start writing the C++ source file

    with open(source_file_name, "w") as f:
        f.write('#include <vector>\n#include "resources.hpp"\nnamespace resources{')

        f.write("const std::vector<unsigned char> {}={{".format(var_name))

        # Append the binary data to the C++ source file

        for byte in binary_data:
            f.write("0x{:02x},".format(byte))
        # Finish writing the C++ source file

        f.write("};}")


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print(
            "Usage: python {} <binary_file_name> <output_path> <var_name>".format(
                sys.argv[0]
            )
        )
        sys.exit(1)
    embed_binary_as_source(sys.argv[1], sys.argv[2], sys.argv[3])
