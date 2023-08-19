project(
    'work',
    'c',
    license : 'MIT',
    version : '0.1.0',
    default_options : [
        'c_std=c99',
        'warning_level=3',
    ],
    meson_version : '>=0.50.0',
)

add_global_arguments([
    '-DSOME_TOKEN=value',
    '-g',
    ], language: 'c')

bib = shared_library (
    'bib',
    ['tree45.c'],
    install : true,
)

executable('main', 'main.c', link_with : bib)
