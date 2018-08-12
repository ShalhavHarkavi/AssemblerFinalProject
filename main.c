/************************************
 *By Matan Liber and Shalhav Harkavi*
 ************************************/

#include "additionalFuncs.h"
static void secondPass(FILE *input, label *head, lines *linesMapHead);

void secondPass(FILE *input, label *head, lines *linesMapHead) {
	unsigned char numOfNames;
	if (numOfNames = hasDirect(linesMapHead -> instWord)) {
		char line[MAX_LINE_LENGTH];
		fseek(input, linesMapHead -> filePos, SEEK_SET);
		fgets(line, MAX_LINE_LENGTH, input);
		for (int i=0; i<numOfNames; i++) {
			char Name[MAX_NAME_LENGTH];
			label *nameLabel;
			if (getName(line, Name)) {
				nameLabel = head;
				while (nameLabel = findLabel(Name, nameLabel)) {
					if (nameLabel -> addId == entry)
						continue;
					else {
						updateAddress(nameLabel, linesMapHead -> instWord, linesMapHead -> position);
						break;
					}
				}
				if (nameLabel == NULL)
					error(12);
			}
			else
				error(10);
		}
	}
	if (linesMapHead -> next != NULL)
		secondPass(input, head, linesMapHead -> next);
	updateEntries(head, head);
}

int assembler(char *fileName)
{
	char line[MAX_LINE_LENGTH/*Placeholder, will find solution for dynamic sized line later.*/], lineName[MAX_NAME_LENGTH];
	FILE* input, output, entries, externals;
	label *head = NULL, temp = NULL;
	unsigned int lineCounter = 0;
	lines *linesMapHead = (lines*)malloc(sizeof(lines));
	lines *currentLine = linesMapHead;
	input = fopen(strcat(fileName, ".as"), "r");
	if (input == NULL)
	{
		fprintf(stderr, "FAILED TO OPEN THE FILE NAMED %s.as\n", fileName);
		return 0; /*Return instead of exit() because maybe we should just skip files that don't work and try to assemble the ones that do, as return will just skip the current not-working file and move on, and exit() will stop the program entirely if I'm not wrong.*/
	}
	output = fopen(strcat(fileName, ".ob"), "w");
	entries = fopen(strcat(fileName, ".ent"), "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
	externals = fopen(strcat(fileName, ".ext"), "w"); /*At the end of the assembler function, if pointer is NULL -> delete file using remove() function.*/
	head = (label*)malloc(sizeof(label));
	temp = head;
	while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
	{
		currentLine -> lineNum = lineCounter;
		currentLine -> filePos = ftell(input) - strlen(line) -1;
		if (isLabel(line) == true)
		{
			name = getLabelName(line);
			temp -> name = lineName;
			temp.id = getType(line);
			temp.addId = getType(line);
			temp -> value = getValue(line, head.id);
			temp -> string = getString(line, head.id);
			temp -> next = (label*)malloc(sizeof(label));
			if (temp.addId == noneAdd) {
				currentLine -> memType = DCline;
				Data(temp, currentLine); /*Creates a word/multiple words for the stored datas*/
				lines *newLine = (lines)malloc(sizeof(lines));
				currentLine -> next = newLine;
				newLine -> next = NULL;
				currentLine =  newLine;
			}
			temp = temp -> next;
		}
		else if (isInstructionLabel(line) == true)
		{
			currentLine -> memType = ICline;
			name = getLabelName(line);
			temp -> name = lineName;
			instruction(line+strlen(name)+1, temp, currentLine);
			temp -> next = (label*)malloc(sizeof(label));
			temp = temp -> next;
			lines *newLine = (lines)malloc(sizeof(lines));
			currentLine -> next = newLine;
			newLine -> next = NULL;
			currentLine =  newLine;
		}
		else
		{
			currentLine -> memType = ICline;
			instruction(line, NULL, currentLine);
			lines *newLine = (lines)malloc(sizeof(lines));
			currentLine -> next = newLine;
			newLine -> next = NULL;
			currentLine =  newLine;
		}
		lineCounter++;
	}
	updateLineList(linesMapHead);
	updateLabelAddress(head);
	secondPass(input, head, linesMapHead);
	fclose(input);
	temp = head;
	while (temp != NULL)
	{
		if (temp.addId == external)
			fprintf(externals, "%s\t%d", temp -> name, temp.adress); /*NEED TO FIND A WAY TO MAKE COLUMNS FOR NAMES AND ADRESSES*/
		if (temp.addId == entry)
			fprintf(entries, "%s\t%d", temp -> name, temp.adress); /*NEED TO FIND A WAY TO MAKE COLUMNS FOR NAMES AND ADRESSES*/
		temp = temp -> next;
	}
	if (entries == NULL)
	{
		fclose(entries);
		remove(entries);
	}
	if (externals == NULL)
	{
		fclose(externals);
		remove(externals);
	}
}

int main(int argc, char *argv[])
{
  /*NEED HERE TO ACTIVATE ASSEMBLER UNTIL FINISHED WITH ARGV BY SENDING THE NAMES ONE BY ONE*/
}
