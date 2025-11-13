;; -*- lexical-binding: t; -*-

(TeX-add-style-hook
 "macros"
 (lambda ()
   (setq TeX-command-extra-options
         "-shell-escape")
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("book" "11pt" "twoside")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("enumitem" "") ("ifthen" "") ("array" "") ("infwarerr" "") ("fixltx2e" "") ("doxygen" "") ("graphicx" "") ("inputenc" "utf8") ("makeidx" "") ("textcomp" "") ("wasysym" "nointegrals") ("ifxetex" "") ("babel" "french") ("emoji" "") ("newunicodechar" "") ("fontenc" "T1") ("mathdesign" "adobe-utopia") ("stickstootext" "p" "osf") ("newtxmath" "stix2" "vvarbb") ("microtype" "tracking") ("geometry" "") ("changepage" "") ("fancyhdr" "") ("natbib" "") ("tocloft" "titles") ("hyperref" "pdftex" "pagebackref=true" "ps2pdf") ("caption" "") ("etoc" "deeplevels") ("etoc_doxygen" "deeplevels")))
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "href")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (TeX-run-style-hooks
    "latex2e"
    "book"
    "bk11"
    "enumitem"
    "ifthen"
    "array"
    "infwarerr"
    "fixltx2e"
    "doxygen"
    "graphicx"
    "inputenc"
    "makeidx"
    "textcomp"
    "wasysym"
    "ifxetex"
    "babel"
    "emoji"
    "newunicodechar"
    "fontenc"
    "mathdesign"
    "stickstootext"
    "newtxmath"
    "microtype"
    "geometry"
    "changepage"
    "fancyhdr"
    "natbib"
    "tocloft"
    "hyperref"
    "caption"
    "etoc"
    "etoc_doxygen")
   (TeX-add-symbols
    '("numberline" 1)
    '("sectionmark" 1)
    '("chaptermark" 1)
    '("doxygenemoji" 2)
    "IfFormatAtLeastTF"
    "doxynormalparskip"
    "doxytocparskip"
    "hrulefilll"
    "clearemptydoublepage"
    "mypdfximage"
    "pdfximage"
    "doxynewunicodechar"
    "footnotesize"
    "scriptsize")
   (LaTeX-add-pagestyles
    "fancyplain"
    "plain"))
 :latex)

