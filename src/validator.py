"""CyanoByte Validator

The CyanoByte validator is used to ensure that a CyanoByte
document meets the specification.
"""

import sys
import json
import click
import yaml
from jsonschema import validate


def cyanobyte_valdiate(input_files):
    """
    Valdiate a list of CyanoByte documents.

    Args:
        input_files: A list of CyanoByte documents to validate.
    """
    # Load the JSON Schema file
    with open("spec/cyanobyte.schema.json", "r") as schema_json:
        schema = json.load(schema_json)

    # Validate each document against the schema
    for input_file in input_files:
        with open(input_file, "r") as document_yaml:
            document_dict = yaml.load(document_yaml)
            validate(instance=document_dict, schema=schema)


@click.command()
@click.option("-i", "--input", "input_files", multiple=True)
def click_valdiate(input_files):
    """
    Main command line entrypoint

    Args:
        input_files: A list of CyanoByte documents to validate.
    """
    cyanobyte_valdiate(input_files)


if __name__ == "__main__":
    click_valdiate(sys.argv[1:])
