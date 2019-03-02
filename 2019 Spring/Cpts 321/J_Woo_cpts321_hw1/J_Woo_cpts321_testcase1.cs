using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace testcs
{
    class testcase
    {
        public void simpletest1()
        {
            tree a = new tree();
            Assert.AreEqual(a.addtree(1), true);
            Assert.AreEqual(a.printall(), 1);
        }
        public void simpletest2() {
            tree a = new tree();
            Assert.AreEqual(a.printall(), 0);
        }
    }
}
