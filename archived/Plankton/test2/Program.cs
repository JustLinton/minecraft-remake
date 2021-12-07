using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Plankton;

namespace test2
{
    class Program
    {
        static void Main(string[] args)
        {
            PlanktonMesh pMesh = new PlanktonMesh();
            List<PlanktonXYZ> vertices = new List<PlanktonXYZ>();
            vertices.Add(new PlanktonXYZ(-1, 1, 0));
            vertices.Add(new PlanktonXYZ(1, 1, 0));
            vertices.Add(new PlanktonXYZ(1, -1, 0));
            vertices.Add(new PlanktonXYZ(-1, -1, 0));

            pMesh.Vertices.AddVertices(vertices);

            pMesh.Faces.AddFace(0, 1, 2);
            pMesh.Faces.AddFace(2,0,3);
        }
    }
}
