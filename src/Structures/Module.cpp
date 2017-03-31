#include "Module.h"
Module::Module(std::string i,std::string o) : infile(i), outfile(o)
{
    
}

void Module::build()
{
        lex.construct(infile);
        std::cout<<lex.getOutput().getList()[0].getType()<<lex.getOutput().getList()[1].getType()<<std::endl;
        std::cout<<lex.getOutput().getList()[0].getValue()<<lex.getOutput().getList()[1].getValue()<<std::endl;
        tree.build(lex.getOutput());
        //then settle dependencies
}
