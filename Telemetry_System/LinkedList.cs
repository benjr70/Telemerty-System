using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Telemetry_System
{

    public class ListNode
    {
        public inputdata data;
        public ListNode next;
    }

    public class LinkedList
    {
        public ListNode head;


        public void AppendNode(inputdata newnode)
        {
            if(head == null)
            {
                head = new ListNode();
                head.data = newnode;
                head.next = null;
                
            }
            else
            {

                ListNode toAdd = new ListNode();
                toAdd.data = newnode;
                ListNode current = head;
                while(current.next != null)
                {
                    current = current.next;
                }
                current.next = toAdd;
                
            }



        }
        public string printNodes()
        {
            ListNode current = head;
            string datastring = "";
            while(current != null)
            {
                datastring += current.data.RotaryEcoder + "\t" + current.data.RotaryEcoder2 + "\t" + current.data.ax + "\t" + current.data.ay + "\t" + current.data.az + "\t" + current.data.gx + "\t" + current.data.gy + "\t" + current.data.gz + "\t" + current.data.mx + "\t" + current.data.my + "\t" + current.data.mz + "\t" + current.data.distance1 + "\t" + current.data.distance2 + "\t" + current.data.distance2 + "\t" + current.data.distance3 + "\t" + current.data.distance4 + "\r\n";
                current = current.next;
            }

            return datastring;
        }

    }
}
