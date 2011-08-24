/*
 *  Copyright (C) 2011  Regents of the University of Michigan
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GenomeSequence.h"
#include "Parameters.h"

int main(int argc, char ** argv)
{
    bool force = false;
    bool mmap = true;

    ParameterList inputParameters;
    BEGIN_LONG_PARAMETERS(longParameterList)
        LONG_PARAMETER("force", &force)
        LONG_PARAMETER("mmap", &mmap)
        END_LONG_PARAMETERS();
   
    inputParameters.Add(new LongParameters ("Input Parameters", 
                                            longParameterList));
    
    int numArgsProcessed = inputParameters.ReadWithTrailer(argc, argv);

    // numArgsProcessed does not include the program name, so add one to it.
    ++numArgsProcessed;

    if(numArgsProcessed == argc)
    {
        // The fasta file was not specified.
        std::cerr << "Create the UM reference from the .fa file.\n";
        std::cerr << "Usage: createUMref [--mmap] [--force] refFileName.fa\n"
                  << "--mmap : toggle whether to use memory map "
                  << "(default: true)\n"
                  << "--force: toggle whether to force creation even if it "
                  << "already exists\n"
                  << "\n";
        return(-1);
    }

    bool failure = false;

    for(int i = numArgsProcessed; i < argc; i++)
    {
        // Create reference object.
        GenomeSequence genomeSequence;
        genomeSequence.setApplication("createUMref");
        genomeSequence.setReferenceName(argv[i]);

        genomeSequence.setCreateOverwrite(force);
        genomeSequence.useMemoryMap(mmap);
        
        // open and create one if the index does not exist
        if(genomeSequence.create())
        {
            std::cerr << "Failed to create the UM-type reference file: "
                      << argv[i] << std::endl;
            failure = true;
        }
        else
        {
            // The UM index was successfully created.
            std::cerr << "Successfully created the UM-type reference file: "
                      << argv[i] << std::endl;
        }
    }
    
    if(failure)
    {
        return(-1);
    }
    // All conversions were successful.
    return(0);
}
