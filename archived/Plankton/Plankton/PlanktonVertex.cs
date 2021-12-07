using System;

namespace Plankton
{
    /// <summary>
    /// Represents a vertex in Plankton's halfedge mesh data structure.
    /// 半边数据结构网格中的顶点结构
    /// </summary>
    public class PlanktonVertex
    {
        /// <summary>
        /// 以该点为起点的一个半边的索引
        /// </summary>
        public int OutgoingHalfedge;
        
        /// <summary>
        /// 默认构造函数
        /// </summary>
        internal PlanktonVertex()
        {
            this.OutgoingHalfedge = -1;//单独创建的顶点无出半边，表示未设置点（死点）
        }
        
        /// <summary>
        /// 重载构造函数
        /// </summary>
        /// <param name="x">x坐标</param>
        /// <param name="y">y坐标</param>
        /// <param name="z">z坐标</param>
        internal PlanktonVertex(float x, float y, float z)//创建带坐标值的死点
        {
            OutgoingHalfedge = -1;
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

        /// <summary>
        /// 重载构造函数
        /// </summary>
        /// <param name="x">x坐标</param>
        /// <param name="y">y坐标</param>
        /// <param name="z">z坐标</param>
        internal PlanktonVertex(double x, double y, double z)//双精度坐标
            : this((float) x, (float) y, (float) z)
        {
            // empty
        }

        /// <summary>
        /// X坐标
        /// </summary>
        public float X { get; set; }
        /// <summary>
        /// Y坐标
        /// </summary>
        public float Y { get; set; }
        /// <summary>
        /// Z坐标
        /// </summary>
        public float Z { get; set; }

        /// <summary>
        /// 返回顶点坐标
        /// </summary>
        /// <returns>顶点坐标</returns>
        public PlanktonXYZ ToXYZ()//顶点转向量
        {
            return new PlanktonXYZ(this.X, this.Y, this.Z);
        }

        /// <summary>
        /// Gets an unset PlanktonVertex. Unset vertices have an outgoing halfedge index of -1.
        /// 获得一个新死点，死点的出半边索引均为-1
        /// </summary>
        public static PlanktonVertex Unset //只读，能生产新的死点
        {
            get { return new PlanktonVertex() { OutgoingHalfedge = -1 }; }
        }
        
        /// <summary>
        /// Whether or not the vertex is currently being referenced in the mesh.
        /// 判断该点是否为死点，true为正，false为负
        /// </summary>
        public bool IsUnused { get { return (this.OutgoingHalfedge < 0); } }
        
        [Obsolete()]
        public bool Dead { get { return this.IsUnused; } }
    }
}
