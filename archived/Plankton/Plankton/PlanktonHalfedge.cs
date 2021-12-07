using System;

namespace Plankton
{
    /// <summary>
    /// Represents a halfedge in Plankton's halfedge mesh data structure.
    /// </summary>
    public class PlanktonHalfedge
    {
        /// <summary>
        /// 半边起始点
        /// </summary>
        public int StartVertex;
        /// <summary>
        /// 半边邻接面
        /// </summary>
        public int AdjacentFace;
        /// <summary>
        /// 下一个条半边
        /// </summary>
        public int NextHalfedge;
        /// <summary>
        /// 上一条半边
        /// </summary>
        public int PrevHalfedge;

        /// <summary>
        /// 默认构造函数
        /// </summary>
        internal PlanktonHalfedge()//单独创建半边为未设置半边（死边）
        {
            StartVertex = -1;
            AdjacentFace = -1;
            NextHalfedge = -1;
            PrevHalfedge = -1;
        }
        
        /// <summary>
        /// 重构构造函数
        /// </summary>
        /// <param name="Start">起始点索引</param>
        /// <param name="AdjFace">邻接面索引</param>
        /// <param name="Next">下一条半边索引</param>
        internal PlanktonHalfedge(int Start, int AdjFace, int Next)//创建带起始顶点，邻接面，下一条半边的半边
        {
            StartVertex = Start;
            AdjacentFace = AdjFace;
            NextHalfedge = Next;
        }
        
        /// <summary>
        /// Gets an Unset PlanktonHalfedge.
        /// 属性，可以生产出新的死边
        /// </summary>
        public static PlanktonHalfedge Unset
        {
            get
            {
                return new PlanktonHalfedge()
                {
                    StartVertex = -1,
                    AdjacentFace = -1,
                    NextHalfedge = -1,
                    PrevHalfedge = -1
                };
            }
        }
        
        /// <summary>
        /// <para>Whether or not the vertex is currently being referenced in the mesh.</para>
        /// <para>Defined as a halfedge which has no starting vertex index.</para>
        /// 判断是否是死边
        /// </summary>
        public bool IsUnused { get { return (this.StartVertex < 0); } }
        
        [Obsolete()]
        public bool Dead { get { return this.IsUnused; } }
    }
}
