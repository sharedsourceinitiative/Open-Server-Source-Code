" File:         latex.vim
" Last update:	Mon Mar 01 14:39:38 CET 1999
" Availability: This file is available as
"   <URL:http://www.math.fu-berlin.de/~guckes/vim/source/latex.vim>
"   <URL:http://www.vim.org/source/latex.vim> (mirror)
" Size:         ...
" Purpose:      Setup file for the editor Vim (Vi IMproved)
" 		Abbreviations and mappings for writing texts in LaTeX
" Usage:	From \begin{TEST}within Vim use\end{TEST} the command ":source latex.vim".
" Author:       Sven Guckes guckes@vim.org (guckes@math.fu-berlin.de)
"               <URL:http://www.math.fu-berlin.de/~guckes/sven/>
" See also:	http://www.math.fu-berlin.de/~guckes/latex/
" ===================================================================
" Enclose current visual text (incomplete lines) with environment "ENVNAME"
    map  ,Env `>a\end{ENVNAME}<esc>`<i\begin{ENVNAME}<esc>
   vmap  ,Env <c-o>,Env
" Enclose current visual text (  complete lines) with environment "ENVNAME"
    map  ,ENV `>o\end{ENVNAME}<esc>`<O\begin{ENVNAME}<esc>
   vmap  ,ENV <c-o>,ENV
" Center current visual text
"  vmap  ,Center <esc>iab ENVNAME center|:normal ,Env<c-m>
"  `<O\begin{center}<esc>`>o\end{center}<esc>
"
"   Inserting pairs of quotes:
    map  Y"  ``''<Left><Left>
"EOF
