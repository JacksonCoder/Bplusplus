#ifndef PARSING_H_INCLUDED
#define PARSING_H_INCLUDED
#include "../Structures/File.h"
#include "../Structures/ASTTree.h"

enum IndentationType
{
    SPACES,
    TABS
};
IndentationType getIndentationType(File* bfile)
{
    for(int lines_i = 0; lines_i < bfile->lines.size();lines_i++)
    {
        if(bfile->lines[lines_i][0] == ' ')
        {
            return SPACES;
        }
        if(bfile->lines[lines_i][0] == '\t')
        {
            return TABS;
        }
    }
    return SPACES; //default
}
void stripWhitespace(File* file,IndentationType IT)
{
    /*
    switch(IT)
    {
    case SPACES:
        {

            for(int lines_i = 0; lines_i < file->lines.size();lines_i++)
            {
                std::string &l = file->lines[lines_i];

                std::string::iterator startingpoint = l.begin();
                while(*startingpoint == ' ')
                {
                    startingpoint++;
                }
                l.erase(std::remove(l.begin(),startingpoint,'\t'),l.end());
            }
            break;
        }
    case TABS:
        {
            for(int lines_i = 0; lines_i < file->lines.size();lines_i++)
            {
                std::string &l = file->lines[lines_i];
                l.erase(std::remove(l.begin(),l.end(),' '),l.end());
                std::string::iterator startingpoint = l.begin();
                while(*startingpoint == '\t')
                {
                    startingpoint++;
                }
            }
            break;
        }
    }
    */
}
ASTTree assembleTree(File* bfile)
{

    ASTTree returnValue;

    ASTNode* attacher = returnValue.getRoot();

    int scopelevel = 0;

    for(int i = 0; i < bfile->lines.size();i++){
        int numberofindents = 0;
        std::string::iterator startingpoint = bfile->lines[i].begin();

                if(getIndentationType(bfile) == TABS) while(*startingpoint == '\t')
                {
                    startingpoint++;
                }
                else while(*startingpoint == ' ')
                {
                    startingpoint++;
                }
        if(getIndentationType(bfile) == TABS) numberofindents = std::count(bfile->lines[i].begin(),startingpoint,'\t');
        else numberofindents = std::count(bfile->lines[i].begin(),startingpoint,' ');
                std::cout<<numberofindents<<std::endl;
        if(numberofindents > scopelevel){

            scopelevel++;

            if(i>0){ attacher = attacher->branches[attacher->branches.size()-1]; }
            else
            {
                fail("Error: unexpected lack of indentation!");
            }
            if(numberofindents > scopelevel)
                {
                fail("Error: unexpected lack of indentation!");
            }
        }
        if(numberofindents < scopelevel){

            scopelevel--;

            if(i>0){ attacher = attacher->parent; }
            else
            {
                fail("Error: unexpected lack of indentation!");
            }
            if(numberofindents > scopelevel)
                {
                fail("Error: unexpected lack of indentation!");
            }
        }
        returnValue.addNodeTo(attacher,bfile->lines[i]);
    }
    returnValue.setup();
    returnValue.metaSetup();
    return returnValue;
}

void preParseContents(File* bfile)
{

    IndentationType indentType = getIndentationType(bfile);
    if(indentType ==3 ){
        fail("Could not successfully identify primary indentation type");
    }
    stripWhitespace(bfile,indentType);
}

#endif // PARSING_H_INCLUDED
