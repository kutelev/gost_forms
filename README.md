[![pipeline status](https://gitlab.com/kutelev/gost_forms/badges/master/pipeline.svg)](https://gitlab.com/kutelev/gost_forms/commits/master)

В проекте приведены примеры некоторых текстовых документов описанных в ГОСТ 2.106. На текстовые документы накладываются основные надписи, выполненные в соответствии с ГОСТ 2.104.

Для того чтобы собрать все документы необходима ОС Linux (ранее проверялось только на дистрибутивах Debian и Ubuntu) со следующими установленными пакетами:
* g++
* make
* inkscape
* pdftk
* xelatex

Полный набор необходимых пакетов (справедливо для Ubuntu 14.04):
* g++
* inkscape
* locales
* make
* msttcorefonts
* pdftk
* texlive-fonts-recommended
* texlive-lang-cyrillic
* texlive-latex-extra
* texlive-latex-recommended
* texlive-xetex

Также необходимо наличие некоторых шрифтов, сами шрифты имеются в данном репозитории. Для того чтобы они стали доступны обычно достаточно просто поместить их в директорию ~/.fonts.

В данном репозитории находится образец Dockerfile'а: https://gitlab.com/kutelev/gost_forms/raw/master/Dockerfile

Готовый к использованию образ может быть получен следующим образом: docker pull registry.gitlab.com/kutelev/gost_forms:build

Примеры собранных документов доступны по ссылке: https://gitlab.com/kutelev/gost_forms/builds/artifacts/master/browse?job=build:documents
