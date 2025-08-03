
.PHONY: todo
todo:
	@find . -regextype egrep ! -regex "\./\.git.*" ! -regex "\./dev/Makefile2.inc" -type f | xargs grep -i "todo"
