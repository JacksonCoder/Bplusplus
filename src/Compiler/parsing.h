#ifndef PARSING_H_INCLUDED
#define PARSING_H_INCLUDED
#include "Structures/BPPError.h"

enum IndentationType
{
    SPACES,
    TABS
};
IndentationType getIndentationType(BPPFile* bfile)
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
BPPError stripWhitespace(BPPFile* file,IndentationType IT)
{

    switch(IT)
    {
    case SPACES:
        {

            for(int lines_i = 0; lines_i < file->lines.size();lines_i++)
            {
                std::string &l = file->lines[lines_i];
                l.erase(std::remove(l.begin(),l.end(),'\t'),l.end());
                std::string::iterator startingpoint = l.begin();
                while(*startingpoint == ' ')
                {
                    startingpoint++;
                }
                l.erase(std::remove(startingpoint,l.end(),' '),l.end());
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
                l.erase(std::remove(startingpoint,l.end(),'\t'),l.end());
            }
            break;
        }
    }
    BPPError noerror;
    return noerror;
}
BPPTokenTree assembleTree(BPPFile* bfile)
{

    BPPTokenTree returnValue;

    BPPTNode* attacher = returnValue.getRoot();

    int scopelevel = 0;

    for(int i = 0; i < bfile->lines.size();i++){
        int numberofindents;
        if(getIndentationType(bfile) == TABS) numberofindents = std::count(bfile->lines[i].begin(),bfile->lines[i].end(),'\t');
        else numberofindents = std::count(bfile->lines[i].begin(),bfile->lines[i].end(),' ');
        if(numberofindents > scopelevel){

            scopelevel++;

            if(i>0){ attacher = attacher->branches[attacher->branches.size()-1]; }
            else
            {
                returnValue.getError().setError(true);
                returnValue.getError().setMessage("On line " + std::to_string(i) + ": Error: unexpected indentation!");
                return returnValue;
            }
            if(numberofindents > scopelevel)
                {
                returnValue.getError().setError(true);
                returnValue.getError().setMessage("Error: unexpected indentation!");
                return returnValue;
            }
        }
        if(numberofindents < scopelevel){

            scopelevel--;

            if(i>0){ attacher = attacher->parent; }
            else
            {
                returnValue.getError().setError(true);
                returnValue.getError().setMessage("Error: unexpected lack of indentation!");
                return returnValue;
            }
            if(numberofindents > scopelevel)
                {
                returnValue.getError().setError(true);
                returnValue.getError().setMessage("Error: unexpected lack of indentation!");
                return returnValue;
            }
        }
        returnValue.addNodeTo(attacher,bfile->lines[i]);
    }

    return returnValue;
}

BPPError preParseContents(BPPFile* bfile)
{

    IndentationType indentType = getIndentationType(bfile);
    if(indentType ==3 ){
        return BPPError("Could not successfully identify primary indentation type");
    }
    BPPError ste = stripWhitespace(bfile,indentType);
    return ste;
}

#endif // PARSING_H_INCLUDED
