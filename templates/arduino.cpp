{% import 'macros.jinja2' as utils %}
{% import 'clang.jinja2' as cpp %}
{% set template = namespace(enum=false, sign=false, math=false, struct=false) %}
/*
{{ utils.pad_string('* ', utils.license(info.copyright.name, info.copyright.date, info.license.name)) -}}
*
* Auto-generated file for {{ info.title }} v{{ info.version }}.
* Generated from {{ fileName }} using Cyanobyte Codegen v{{ version }}
* Class for {{ info.title }}
{{utils.pad_string("* ", info.description)}}
*/

{% macro logic(logicalSteps, function) -%}

{% for step in logicalSteps %}
{% for key in step.keys() %}
{# // Check if assignment is a send-op #}
{% if key == 'cmdWrite' %}
    write{{step[key].register[12:]}}({{step[key].value}});
    {% break %}
{% endif %}
{# // Check if assignment op #}
{% if step[key][0:1] == "=" %}
    {{key}} {{step[key]}};
{% endif %}
{# // Check if assignment is a send-op #}
{% if key == 'send' %}
    write{{function.register[12:]}}({{step[key]}});
{% endif %}
{# // Check if assignment is register read op #}
{% if step[key][:12] == '#/registers/' %}
    {{key}} = read{{step[key][12:]}}();
{% endif %}
{# // Check if assignment is function call op #}
{% if step[key][:12] == '#/functions/' %}
    {{key}} = {{step[key] | regex_replace('#/functions/(?P<function>.+)/(?P<compute>.+)', '\\g<function>\\g<compute>')}}();
{% endif %}
{# // If the value is a list, then this is a logical setter #}
{% if step[key] is iterable and step[key] is not string %}
    {{key}} = {{cpp.recursiveAssignLogic(step[key][0], step[key][0].keys()) -}};
{% endif %}
{% endfor %}
{% endfor %}
{%- endmacro %}

{% if i2c.endian == 'little' %}
static short _swap_endian(short val) {
    // Swap the endianness of a short only
    return (val & 0xFF00) >> 8 | (val & 0xFF) << 8;
}
{% endif %}

{# Add signing function if needed #}
{% for register in registers %}
{% for key in register.keys() %}
{% if register[key].signed %}
{% if template.sign is sameas false %}
static short _sign(short val, char length) {
    // Convert unsigned integer to signed integer
    if (val & (1 << (length - 1))) {
        return val - (1 << length);
    }
    return val;
}
{% set template.sign = true %}
{% endif %}
{% endif %}
{% endfor %}
{% endfor %}

#include "{{info.title}}.h"
{% if i2c.address is number %}
#define DEVICE_ADDRESS {{i2c.address}}
{% endif %}

{% for register in registers %}
{% for key in register.keys() %}
#define REGISTER_{{key.upper()}} {{register[key].address}}
{% endfor %}
{% endfor %}

{% if i2c.address is iterable and i2c.address is not string %}
{{info.title}}::{{info.title}}(TwoWire& wire, deviceAddress_t address) :
    _wire(&wire),
    DEVICE_ADDRESS ( address )
{
}
{% else %}
{{info.title}}::{{info.title}}(TwoWire& wire) :
    _wire(&wire)
{
}
{% endif %}

void {{info.title}}::begin() {
    _wire->begin();
}

void {{info.title}}::end() {
    _wire->end();
}

{% for register in registers -%}
{% for key in register.keys() %}
{% set length = register[key].length %}
{% set bytes = (register[key].length / 8) | round(1, 'ceil') | int %}
{{cpp.numtype(length)}} {{info.title}}::read{{key}}() {
    uint8_t datum;
    {{cpp.numtype(length)}} value;
    _wire->beginTransmission(DEVICE_ADDRESS);
    _wire->write(REGISTER_{{key.upper()}});
    if (_wire->endTransmission(false) != 0) {
        return -1;
    }

    if (_wire->requestFrom(DEVICE_ADDRESS, {{length}}) != {{length}}) {
        return 0;
    }

    {# Read a byte at a time #}
    {% for n in range(bytes) %}
    datum = _wire->read();
    value = value << 8 | datum;
    {% endfor %}

    return value;
}

int {{info.title}}::write{{key}}({{cpp.numtype(length)}} data) {
    _wire->beginTransmission(DEVICE_ADDRESS);
    // Put our data into uint8_t buffer
    uint8_t buffer[{{bytes + 1}}] = { (uint8_t) REGISTER_{{key.upper()}} };
    {% for n in range(bytes) %}
    buffer[{{n + 1}}] = (data >> {{8 * (bytes - n - 1)}}) & 0xFF;
    {% endfor %}
    _wire->write(buffer, {{bytes + 1}});
    if (_wire->endTransmission() != 0) {
        return 0;
    }
    return 1;
}

{% endfor %}
{%- endfor %}

{% for field in fields %}
{% for key in field.keys() %}
{% if 'R' is in(field[key].readWrite) %}
{# Getter #}
{{cpp.registerSize(registers, field[key].register[12:])}} {{info.title}}::get{{key}}() {
    // Read register data
    // '#/registers/{{field[key].register[12:]}}' > '{{field[key].register[12:]}}'
    uint8_t val = read{{field[key].register[12:]}}();
    // Mask register value
    val = val & {{utils.mask(field[key].bitStart, field[key].bitEnd)}};
    {% if field[key].bitEnd %}
    // Bitshift value
    val = val >> {{field[key].bitEnd}};
    {% endif %}
    return val;
}
{% endif -%}

{%- if 'W' is in(field[key].readWrite) %}
{# Setter #}

int {{info.title}}::set{{key}}(uint8_t data) {
    {% if field[key].bitEnd %}
    // Bitshift value
    data = data << {{field[key].bitEnd}};
    {% endif %}
    // Read current register data
    // '#/registers/{{field[key].register[12:]}}' > '{{field[key].register[12:]}}'
    uint8_t register_data = read{{field[key].register[12:]}}();
    register_data = register_data | data;
    return write{{field[key].register[12:]}}(register_data);
}
{% endif %}
{% endfor %}
{% endfor %}

{% for function in functions %}
{% for key in function.keys() %}
{% for compute in function[key].computed %}
{% for computeKey in compute.keys() %}
{% if compute[computeKey].input %}
{{cpp.returnType(compute[computeKey])}} {{info.title}}::{{key}}{{computeKey}}({{cpp.params(compute[computeKey])}}) {
{% else %}
{{cpp.returnType(compute[computeKey])}} {{info.title}}::{{key}}{{computeKey}}({{cpp.params(compute[computeKey])}}) {
{% endif %}
    {# Declare our variables #}
{{ cpp.variables(compute[computeKey].variables) }}

    {# Read `value` if applicable #}
    {%- for variable in compute[computeKey].input %}
    {% for varKey in variable.keys() %}
    {% if varKey == 'value' %}
    // Read value of register into a variable
    {{cpp.numconv(variable[varKey])}} value = read{{function[key].register[12:]}}();
    {% endif %}
    {% endfor %}
    {% endfor -%}
    {# Handle the logic #}
{{ logic(compute[computeKey].logic, function[key]) }}

    {# Return if applicable #}
    {# Return a tuple #}
    {% if compute[computeKey].return is iterable and compute[computeKey].return is not string %}
    {# In C languages, the array is a parameter `returnArray` you fill #}
    {% for variable in compute[computeKey].return %}
    returnArray[{{loop.index - 1}}] = {{variable}};
    {% endfor %}
    {% endif %}
    {# Return a plain value #}
    {% if compute[computeKey].return is string %}
    return {{compute[computeKey].return}};
    {% endif %}
}

{% endfor %}
{% endfor %}
{% endfor %}
{% endfor %}