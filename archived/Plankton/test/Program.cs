using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Plankton;

namespace test
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
            pMesh.Faces.AddFace(0, 2, 3);

            for (int i = 0; i < pMesh.Halfedges.Count;i++ )
                Console.WriteLine("第"+i+"条边：" + pMesh.Halfedges.GetVertices(i)[0] + "->" + pMesh.Halfedges.GetVertices(i)[1]);

            Console.WriteLine("第1个循环：");
            int j = 0;
           while(true)
           {
               Console.WriteLine(pMesh.Halfedges.GetVertices(j)[0] + "->" + pMesh.Halfedges.GetVertices(j)[1]);
               j = pMesh.Halfedges[j].NextHalfedge;
               if (j == 0)
                   break;
           }

           Console.WriteLine("第2个循环：");
           j = 5;
           while (true)
           {
               Console.WriteLine(pMesh.Halfedges.GetVertices(j)[0] + "->" + pMesh.Halfedges.GetVertices(j)[1]);
               j = pMesh.Halfedges[j].NextHalfedge;
               if (j == 5)
                   break;
           }

           Console.WriteLine("第3个循环：");
           j = 1;
           while (true)
           {
               Console.WriteLine(pMesh.Halfedges.GetVertices(j)[0] + "->" + pMesh.Halfedges.GetVertices(j)[1]);
               j = pMesh.Halfedges[j].NextHalfedge;
               if (j == 1)
                   break;
           }

           Console.WriteLine("第0个面的半边："+pMesh.Faces[0].FirstHalfedge);
           Console.WriteLine("第1个面的半边：" + pMesh.Faces[1].FirstHalfedge);

           for (int i = 0; i < pMesh.Vertices.Count; i++)
               Console.WriteLine("第"+i+"个顶点的出边："+pMesh.Halfedges.GetVertices(pMesh.Vertices[i].OutgoingHalfedge)[0]
                                                        +"->"
                                                        + pMesh.Halfedges.GetVertices(pMesh.Vertices[i].OutgoingHalfedge)[1]);
        }
    }
}
