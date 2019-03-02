//Jeonghyeon Woo
//011618854



using System;

public class tree
{
    public int level;
    public int it;
    public tree first;
    public tree second;

    public tree()
    {
        level = 0;
        it = -1;
        first = null;
        second = null;
    }

    public tree(int a)
    {
        level = a + 1;
        it = -1;
        first = null;
        second = null;
    }

    public bool addtree(int a)
    {//add items to the tree
        if (level == 0)
        {
            level = 1;
            it = a;
            return true;
        }
        if (it < 0)
        {
            it = a;
            return true;
        }
        if (a < it)
        {
            if (first != null)
            {
                return first.addtree(a);
            }
            else
            {
                first = new tree(level);
                return first.addtree(a);
            }
        }
        if (a > it)
        {
            if (second != null)
            {
                return second.addtree(a);
            }
            else
            {
                second = new tree(level);
                return second.addtree(a);
            }
        }
        return false;
    }

    public int printall()
    {//print items according to depth first search, and it eventually returns the maximum level of the tree
        if (level == 0) return 0;
        int a = 0;
        int b = 0;
        if (first != null)
        {
            a = first.printall();
        }
        Console.Write(it);
        Console.Write(' ');
        if (second != null)
        {
            b = second.printall();
        }
        return a > b ? a + 1 : b + 1;
        //choose which path has deeper depth
    }
}



public class Program
{
    public static void interf() {
        tree T = new tree();
        Console.WriteLine("Enter a collection of numbers in the range [0, 100], separated by spaces: ");
        string line = Console.ReadLine();
        string[] input = line.Split(' ');
        int nnodes = 0;
        foreach (var item in input)
        {
            int temp = Int32.Parse(item);
            if (T.addtree(temp)) nnodes++;
        }
        Console.Write("Tree contents: ");
        int maxlevel = T.printall();
        Console.Write("\nTree Statistics:\n Number of nodes: {0}\n Number of levels: {1}\n", nnodes, maxlevel);
        int min = 1;
        int tlevel = 2;
        while (nnodes > tlevel)
        {
            min++;
            tlevel *= 2;
        }//2^n, n is minimum level
        Console.Write(" Minimum number of levels that a tree with {0} nodes could have = {1}\n", nnodes, min);
        Console.ReadLine();
    }
    public static void Main()
    {
        interf();
    }
}