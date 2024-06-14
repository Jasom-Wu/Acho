from django import template
import os

register = template.Library()


@register.filter(name='base_path')
def basePath(path):
    return os.path.basename(path)


@register.filter(name='size_swap')
def sizeSwap(value):
    if value == 0:
        return '0 bit'
    if value < 1024*1024:
        return '{:.2f} Kb'.format(value / 1024)
    else:
        return '{:.2f} Mb'.format(value / (1024*1024))

