FILE_NAME=handout/lock.c

all:
	@echo "There is a separate Makefile in the handout directory."
	@echo
	@echo "Please use 'make handin' in the current directory when you"
	@echo "are ready to submit your work."

handin:
	@echo "Handing in: " $(FILE_NAME)
	@echo "Note the file above -- it is the only one added to the hand-in!"
	@echo "The output below will mention other files (including a Java file!)."
	@echo "This is to be expected."
	@mkdir -p .tmp-handin-directory
	@mkdir -p .tmp-handin-directory/uppgift1
	@mkdir -p .tmp-handin-directory/uppgift2
	@touch .tmp-handin-directory/uppgift2/YourCode.java
	@cp $(FILE_NAME) .tmp-handin-directory/uppgift1/yourcode.c
	@cd .tmp-handin-directory; /it/sw/misc/bin/twhandin
	@rm -rf .tmp-handin-directory
