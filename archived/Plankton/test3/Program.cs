using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Plankton;

namespace test3
{
    class Program
    {
        static void Main(string[] args)
        {
            PlanktonMesh pMesh = new PlanktonMesh();
            List<PlanktonXYZ> vertices = new List<PlanktonXYZ>();
            vertices.Add(new PlanktonXYZ(0, 0, 0));
            vertices.Add(new PlanktonXYZ(0, 1, 0));
            vertices.Add(new PlanktonXYZ(1,-1, 0));
            vertices.Add(new PlanktonXYZ(-1,-1,0));

            pMesh.Vertices.AddVertices(vertices);

            pMesh.Faces.AddFace(0, 1, 2);
            pMesh.Faces.AddFace(0, 2, 3);
            pMesh.Faces.AddFace(0, 3, 1);

            for (int i = 0; i < pMesh.Halfedges.Count; i++)
                Console.WriteLine("第" + i + "条边：" + pMesh.Halfedges.GetVertices(i)[0] + "->" + pMesh.Halfedges.GetVertices(i)[1]);

            Console.WriteLine("第0个顶点的出边为："+pMesh.Halfedges.GetVertices(pMesh.Vertices[0].OutgoingHalfedge)[0]
                                                   +"->"
                                                   + pMesh.Halfedges.GetVertices(pMesh.Vertices[0].OutgoingHalfedge)[1]);
        }
    }
}
