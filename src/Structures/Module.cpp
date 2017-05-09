#include "../../Include/Module.h"
Module::Module(std::string i,std::string o) : infile(i), outfile(o)
{
    
}

void Module::build()
{
        std::cout<<"I am here!"<<std::endl;
        lex.construct(infile);
        for(auto test : lex.getOutput().tokens)
        {
                std::cout<<test.getType();
        }
        tree.build(lex.getOutput());
        std::cout<<tree.getRoot()->getResult()<<std::endl;
        //then settle dependencies
}
