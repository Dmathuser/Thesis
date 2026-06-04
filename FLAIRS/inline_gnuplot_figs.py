#!/usr/bin/env python3
"""
Read FLAIRS.tex, find each gnuplot figure block, and replace it with the
contents of the corresponding cairolatex .tex file.

Handles two cases:
  (a) First-time inline: figure body is a bare
      \\includegraphics[...]{gnuplot_figs/BASENAME.pdf}
  (b) Re-inline: figure body is an already-inlined gnuplot block
      starting with "% GNUPLOT: LaTeX picture with Postscript" and
      ending with "\\endgroup", containing an
      \\includegraphics[...]{gnuplot_figs/BASENAME} (no .pdf).

In both cases the basename identifies the gnuplot_figs/<basename>.tex
file to inline.
"""

import re
import os

TEX_FILE = "FLAIRS.tex"
GP_DIR = "gnuplot_figs"


def read_gnuplot_tex(fig_name):
    path = os.path.join(GP_DIR, fig_name + ".tex")
    with open(path, 'r') as f:
        return f.read()


def make_replacement(fig_name, line_indent):
    """Build the replacement text: each line of the gnuplot .tex prefixed
    with line_indent (matches surrounding indentation). The gnuplot .tex
    references the figure by bare basename; rewrite it to gnuplot_figs/<basename>
    so pdflatex can find the PDF when FLAIRS.tex is the main document."""
    tex_content = read_gnuplot_tex(fig_name).rstrip()
    tex_content = re.sub(
        r'(\\includegraphics\[[^\]]*\]\{)' + re.escape(fig_name) + r'(\})',
        r'\1gnuplot_figs/' + fig_name + r'\2',
        tex_content,
    )
    lines = tex_content.split('\n')
    return '\n'.join(line_indent + line if line else line for line in lines)


def main():
    with open(TEX_FILE, 'r') as f:
        content = f.read()

    # Re-inline case: match already-inlined block from "% GNUPLOT:"
    # through "\endgroup". Capture leading indent of the comment line so
    # we can preserve it.
    inlined_pattern = re.compile(
        r'([ \t]*)% GNUPLOT: LaTeX picture with Postscript.*?\\endgroup',
        re.DOTALL,
    )

    n_re = 0

    def inlined_replacer(m):
        nonlocal n_re
        indent = m.group(1)
        block = m.group(0)
        bn = re.search(
            r'\\includegraphics\[[^\]]*\]\{gnuplot_figs/([^}]+?)(?:\.pdf)?\}',
            block,
        )
        if not bn:
            return block
        fig_name = bn.group(1)
        n_re += 1
        return make_replacement(fig_name, indent)

    new_content = inlined_pattern.sub(inlined_replacer, content)

    # First-time inline case: bare \includegraphics{gnuplot_figs/BASE.pdf}
    original_pattern = re.compile(
        r'([ \t]*)\\includegraphics\[[^\]]*\]\{gnuplot_figs/([^}]+)\.pdf\}'
    )

    n_orig = 0

    def original_replacer(m):
        nonlocal n_orig
        indent = m.group(1)
        fig_name = m.group(2)
        n_orig += 1
        return make_replacement(fig_name, indent)

    new_content = original_pattern.sub(original_replacer, new_content)

    with open(TEX_FILE, 'w') as f:
        f.write(new_content)

    print(f"Inlined {n_re} re-inline + {n_orig} first-time = {n_re + n_orig} figures")


if __name__ == '__main__':
    main()
