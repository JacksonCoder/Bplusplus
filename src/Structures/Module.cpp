#include "../../Include/Module.h"
Module::Module(std::string i,std::string o) : infile(i), outfile(o)
{

}

void Module::build()
{
        lex.construct(infile);
        working_tree = tree;
        tree.build(lex.getOutput());
        //std::cout<<tree.getRoot()->getResult()<<std::endl;
        //then settle dependencies
}
