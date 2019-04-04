{% import 'macros.jinja2' as utils %}
{{ utils.pad_string('# ', utils.license('Google Inc.', '2019', info.license.name)) -}}
#
# Auto-generated file for {{ info.title }} v{{ info.version }}.
# Generated from {{ fileName }} using Cyanobyte Codegen v{{ version }}
"""
Class for {{ info.title }}
"""

import sys
try:
    import smbus
except ImportError:
    print("Fatal error! Make sure to install smbus!")
    sys.exit(1)

{# Create enums as for functions #}
{% for function in functions %}
{% for key in function.keys() %}
{% if function[key].enum %}
{# Optionally import class #}
{% if importedEnum != True %}
from enum import Enum
{% set importedEnum = True %}
{% endif %}
{# Create enum class #}
class {{key[0].upper()}}{{key[1:]}}Values(Enum):
    """
{{utils.pad_string("    ", "Valid values for " + function[key].title)}}
    """
    {% for enumObject in function[key].enum %}
    {% for enumKey in enumObject.keys() %}
    {{enumKey.upper()}} = {{enumObject[enumKey].value}} # {{enumObject[enumKey].title}}
    {% endfor %}
    {% endfor %}
{% endif %}
{% endfor %}
{% endfor %}

{% if i2c.endian == 'little' %}
def _swap_endian(val):
    """
    Swap the endianness of a short only
    """
    return val >> 8 | val << 8
{% endif %}

class {{ info.title }}:
    """
{{utils.pad_string("    ", info.description)}}
    """
    DEVICE_ADDRESS = {{i2c.address}}
    {% for register in registers %}
    {% for key in register.keys() %}
    REGISTER_{{key.upper()}} = {{register[key].address}}
    {% endfor %}
    {% endfor %}

    def __init__(self):
        # Initialize connection to peripheral
        self.bus = smbus.SMBus(1)

    {% for register in registers %}
    {% for key in register.keys() %}
    def get_{{key.lower()}}(self):
        """
{{utils.pad_string("        ", register[key].description)}}
        """
        val = self.bus.read_i2c_block_data(
            self.DEVICE_ADDRESS,
            self.REGISTER_{{key.upper()}}
        )
        {% if i2c.endian == 'little' %}
        val = _swap_endian(val)
        {% endif %}
        return val

    def set_{{key.lower()}}(self, data):
        """
{{utils.pad_string("        ", register[key].description)}}
        """
        {% if i2c.endian == 'little' %}
        data = _swap_endian(data)
        {% endif %}
        self.bus.write_i2c_block_data(
            self.DEVICE_ADDRESS,
            self.REGISTER_{{key.upper()}},
            data
        )
    {% endfor %}
    {% endfor %}

    {% for function in functions %}
    {% for key in function.keys() %}
    {% if 'R' is in(function[key].readWrite) %}
    {# Getter #}
    def get_{{key.lower()}}(self):
        """
{{utils.pad_string("        ", function[key].description)}}
        """
        # Read register data
        # '#/registers/{{function[key].register[12:]}}' > '{{function[key].register[12:]}}'
        val = self.get_{{function[key].register[12:].lower()}}()
        # Mask register value
        val = val & {{utils.mask(function[key].bitStart, function[key].bitEnd)}}
        {% if function[key].bitEnd %}
        # Bitshift value
        val = val >> {{function[key].bitEnd}}
        {% endif %}
        return val
    {% endif %}

     {% if 'W' is in(function[key].readWrite) %}
    {# Setter #}
    def set_{{key.lower()}}(self, data):
        """
{{utils.pad_string("        ", function[key].description)}}
        """
        {% if function[key].bitEnd %}
        # Bitshift value
        data = data << {{function[key].bitEnd}}
        {% endif %}
        # Read current register data
        # '#/registers/{{function[key].register[12:]}}' > '{{function[key].register[12:]}}'
        register_data = self.get_{{function[key].register[12:].lower()}}()
        register_data = register_data | data
        self.set_{{function[key].register[12:].lower()}}(register_data)
    {% endif %}
    {% endfor %}
    {% endfor %}