#!/usr/bin/env python3
"""
Read FLAIRS.tex, find each gnuplot figure \includegraphics line,
and replace it with the contents of the corresponding cairolatex .tex file.
The .tex content is pasted directly into the figure environment.
"""

import re
import os

TEX_FILE = "FLAIRS.tex"
OUT_FILE = "FLAIRS.tex"
GP_DIR = "gnuplot_figs"

def read_gnuplot_tex(fig_name):
    """Read the cairolatex .tex file and return its contents."""
    path = os.path.join(GP_DIR, fig_name + ".tex")
    with open(path, 'r') as f:
        return f.read()

def main():
    with open(TEX_FILE, 'r') as f:
        content = f.read()

    # Match \includegraphics[...]{gnuplot_figs/BASENAME.pdf}
    pattern = r'\\includegraphics\[([^\]]*)\]\{gnuplot_figs/([^}]+)\.pdf\}'

    def replacer(m):
        fig_name = m.group(2)
        tex_content = read_gnuplot_tex(fig_name)
        # Indent the inlined content for readability
        lines = tex_content.rstrip().split('\n')
        indented = '\n'.join('  ' + line for line in lines)
        return indented

    new_content = re.sub(pattern, replacer, content)

    with open(OUT_FILE, 'w') as f:
        f.write(new_content)

    # Count substitutions
    count = len(re.findall(pattern, content))
    print(f"Inlined {count} gnuplot figures into {OUT_FILE}")

if __name__ == '__main__':
    main()
