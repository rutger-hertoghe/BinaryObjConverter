//#include "BinaryWriter.h"
#include "ObjWriter.h"
#include "BinaryWriter.h"


int main()
{
    // TODO: after writing 57 float values (19 vertices), my writer decides to place a random '}' (Ox7d) in ze code, which messes everything up
    BinaryWriter binWriter{ "low_poly_stanford_bunny.obj", "outputFile.bobj" };
    binWriter.WriteBObj();

    ObjWriter objWriter{ "outputFile.bobj", "finalOutput.obj" };
	objWriter.WriteObj();



    return 0;
}