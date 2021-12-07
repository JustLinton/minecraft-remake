using System;

namespace Plankton
{
    /// <summary>
    /// Represents a face in Plankton's halfedge mesh data structure.
    /// 表示半边数据结构网格中的一个面
    /// </summary>
    public class PlanktonFace
    {
        /// <summary>
        /// 半边索引
        /// </summary>
        public int FirstHalfedge;
        
        /// <summary>
        /// 默认构造函数
        /// </summary>
        public PlanktonFace()//单独创建的面片为未设置面片（死面）
        {
            this.FirstHalfedge = -1;
        }
        
        /// <summary>
        /// 重载构造函数
        /// </summary>
        /// <param name="halfedgeIndex">半边索引</param>
        internal PlanktonFace(int halfedgeIndex)//创建带起始半边的面片
        {
            this.FirstHalfedge = halfedgeIndex;
        }
        
        /// <summary>
        /// Gets an unset PlanktonFace. Unset faces have -1 for their first halfedge index.
        /// 属性，能生产出新的死面
        /// </summary>
        public static PlanktonFace Unset
        {
            get { return new PlanktonFace() { FirstHalfedge = -1 }; }
        }
        
        /// <summary>
        /// Whether or not the face is currently being referenced in the mesh.
        /// 获取面的装配状态，true为未装配，false为已装配
        /// </summary>
        public bool IsUnused { get { return (this.FirstHalfedge < 0); } }
        
        [Obsolete()]
        public bool Dead { get { return this.IsUnused; } }
    }
}
