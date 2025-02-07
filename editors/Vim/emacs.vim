" File:         emacs.vim
" Last update:	Wed Feb 03 18:00:00 CET 1999
" Availability: This file is available as
"   <URL:http://www.math.fu-berlin.de/~guckes/vim/source/emacs.vim>
"   <URL:http://www.vim.org/source/emacs.vim> (mirror)
" Size:         "small"  :-)
" Purpose:      Setup file for the editor Vim (Vi IMproved)
" 		Emulate some basic editing commands from the editor "Emacs".
" Usage:	From within Vim use the command ":source emacs.vim".
" Author:       Sven Guckes guckes@vim.org (guckes@math.fu-berlin.de)
"               <URL:http://www.math.fu-berlin.de/~guckes/sven/>
" ===================================================================
  imap <C-A>  <C-O>0
  imap <C-D>  <C-O>x
  imap <C-B>  <left>
  imap <C-E>  <C-O>$
  imap <C-F>  <right>
" The following commands will move the cursor to the next/previous
" line, but I am used to using Vim's prefix completion commands.
" imap <C-N>  <C-O>j
" imap <C-P>  <C-O>k
  imap <C-O>b <C-O>B
  imap <C-O>f <C-O>W
"
"EOF
