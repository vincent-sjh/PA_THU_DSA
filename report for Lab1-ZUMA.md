# Report for Lab1-Zuma

## 1.错误类型: Runtime Error

* 错误原因：play()函数递归调用过程中，确定下删除的区间是$[left,right)$，接下来擦除的部分中如果包含首字符，则left-1会导致这次递归中执行at()函数时会导致异常，从而导致触发Runtime Error的错误。

  ```
  void play(int rank) {
      int left = rank;
      int right = rank;
      char color = a.at(rank);

      while (left >= 0 && a.at(left) == color) --left;
      left += 1;
      while (right < a.size() && a.at(right) == color) ++right;

      int size = right - left;
      if (size >= 3) {
          a.erase(left, size);
          play(left - 1);
  	//擦除的部分中如果包含首字符，则left-1会导致这次递归中执行at()函数时会导致异常
      }
  }
  ```
* 构造思路：依据上文所说，希望构造会导致擦除的部分中包含首字符的样例所以构造如下。

  ```
  ZZ
  1
  1 Z
  ```

## 2.错误类型：Runtime Error

* 错误原因：就算进行上一行对next非负性的判定的判定，但如果消除后变为空串，则执行at()的时候会导致异常，抛出Runtime Error的错误。

  ```
  int next = left;
  if (left - 1 >= 0) next = left - 1;

  play(next);//就算进行了上一行的判定，如果消除后变为空串则执行at()的时候会导致越界访问
  ```
* 构造思路：如上面所说，使得进行擦除后消掉了所有珠子得到空串即可。

  ```
  ZZ
  1
  1 Z
  ```

## 3.错误类型: Time Limit Exceeded

* 错误原因：这份代码正确性没有问题但执行插入操作的时候整体向后移位，复杂度过高，单次移位复杂度为O(n),m次操作复杂度可能会达到O(mn),这个复杂度难以接受。
* 构造思路：随机生成一段达到长度上限（500000）的初始串，再生成最大操作数数量（500000）的插入输入，注意检查输入的合法性，确认无误为有效输入。

  ```
  MSNTPIBTEQLGTZLBYFPCTROTJKDQYIGIVFGVKIKSCEFXK......
  500000
  102078 X
  134570 N
  133033 Q
  146604 Q
  129625 N
  130258 I
  112173 R
  146253 Y
  152039 D
  ...
  ```

## 4.错误类型: Wrong Answer

* 错误原因：找寻每次消去的左边界时，循环终止后没有将left加一，使得在删除段左侧有字符时会向左多删一个元素，导致Wrong Answer。

  ```
  while (left > 0 && a.at(left) == color) --left;//没有将left加一，使得后面会向左多删一个元素
  while (right < a.size() && a.at(right) == color) ++right;
  ```
* 构造思路：使得插入后删除段左侧有字符，就会导致这个字符也被错误消除。

  ```
  AABBAC
  1
  4 B 
  ```

## 5.错误类型：Wrong Answer

* 错误原因：在读入原始串时没有考虑空串的情况，接受空串时会导致Wrong Answer。

  ```
  cin >> a;//正确版本为getline(cin, a);
  ```
* 构造思路：输入空串即可。

  ```

  0
  ```

## 6.错误类型：Wrong Answer

* 错误原因：缺少每块超出最大长度后进行重组的部分，导致如果超出4096的长度限制还不进行重组，最终覆盖下一块的数据导致Wrong Answer。

  ```
  // 块过长，重组（这份代码缺少的部分）
  if (plen[pos.first] >= plen_bound) {
      p2a();
      a2p();
      pos = find(rank);
  }
  ```
* 构造思路：使得在插入（以及消去）过程中有一块的长度会超出最大长度限制即可，建议不需要发生消去，这样可以最直接地导致异常。

  ```
  AABBAABB...AABBCCDDCCDD...CCDD(AABB循环512次+CCDD循环512次，各自长度为2048，占据一个块)
  3000（数量使得插入数加上原来的字符数的总数量超过每个块的最大长度限制4096）
  2040 C
  2040 D
  2040 C
  2040 D
  ...(在同一位置插入C/D,循环1500次)
  ```

## 7.错误类型：Wrong Answer

* 错误原因：向上寻找的过程中遇到一整个空块时，应该一直向上循环直至非空块，或者直至第一个块。

  ```
  //这里应为while而非if,不是只进行一次循环，而是一直循环至非空块或者第一块。
  if (l.second < 0 && l.first >= 0) {
      l.first--;
      if (l.first >= 0)
      l.second += plen[l.first];
  }
  ```
* 构造思路：数据需要至少占用两个块，然后彻底消去中间的某个块，然后再向空块后方块的首位插入新字符，如果本来可以消去空块前面块的后几个字符，这时会导致异常。

  ```
  BBAABB...AABBAACCDDCCDD...CCDD(BBAA循环512次+CCDD循环512次，各自长度为2048，占据一个块)BB
  1025
  2048 C
  2048 D
  2048 C
  2048 D
  ...
  2048 C
  2048 D(在同一位置插入C/D,循环512次，完全消去第二个块)
  2048 A(在块后首个字符处插入本来应该可以导致消去操作的字符，触发异常)

  ```

## 8.错误类型：Wrong Answer

* 错误原因：计算消除的开区间 (l, r) 时，没有循环查找而是只查找一次，无法处理连消的情况导致Wrong Answer。

  ```
  while (1) {
          while (l.first >= 0 && get(l) == ch) {
  		...
          }
          while (r.first < pn && get(r) == ch) {
  		...
          }
          if (dis > 3) {
              eliminated += dis - 1;
              lbound = l;
              rbound = r;
              if (l.first >= 0) {
                  ch = get(l);
                  dis = 1;
              } else {
                  break;
              }
          } else {
              break;
          }
      }//以上为正确写法，但在这份代码中循环体只进行了一次，但实际上应该通过while(1)循环执行
  ```
* 构造思路：只需要构造出现连消的情况即可。

  ```
  AABBAC
  1
  4 B 
  ```

## 9.错误类型：Runtime Error

* 错误原因：在执行消除时缺少对于消除区间在同一行情况的判定，全都默认为在不同行进行操作。这份代码直接当作在不同行处理，如果消除区间在同一行，len的值此时变成l.secong-r.second +1，len<0，plen类型为无符号数，赋值为一个负数（len）后变为一个大正数，最终导致在输出时p2a()的memcpy函数中发生Runtime Error。

```
// 执行消除
    if (eliminated > 0) {
        alen -= eliminated;
        l = lbound;
        r = rbound;
        if (l.first == r.first) {
            int len = plen[r.first] - r.second;
            if (len > 0) {
                memmove(&p[l.first][l.second + 1], &p[r.first][r.second], len);
            }
            plen[l.first] -= eliminated;
	  //这段是相对正确写法缺少的部分，即处理消除区间在同一行的操作
        } else {
            if (l.first >= 0) {
                plen[l.first] = l.second + 1;//这份代码直接当作在不同行处理
            }
            if (r.first < pn) {
                int len = plen[r.first] - r.second;
			//如果消除区间在同一行，len的值此时变成l.secong-r.second +1
			//len<0
                if (len > 0) {
                    memmove(&p[r.first][0], &p[r.first][r.second], len);
                }
                plen[r.first] = len;
			//plen类型为无符号数，赋值为一个负数后变为一个大正数
			//最终导致在输出时p2a()的memcpy函数中发生异常
            }
            for (int i = l.first + 1; i < r.first; i++)
                plen[i] = 0;
        }
    }
```

* 构造思路：使得消去区间在同一行内即可。

  ```
  AABBCC
  1
  3 B
  ```

## 10.错误类型：Wrong Answer

* 错误原因：当消除区间横跨多个块时，要把中间的区域归零，但正确的做法是从l.first+1开始进行归零，而不是l.first,这样会导致删除区域左边界所在的块也被整体清除，导致Wrong Answer。

```
for (int i = l.first; i < r.first; i++)//应从l.first+1开始清空
                plen[i] = 0;
```

* 构造思路：使得初始的消除区间的左右边界在不同的块中，注意不要让串直接清空，这样长度归零之后错误的程序也会产生正确的输出（空串）。

  ```
  AABBAABB...AABB(AABB循环512次，长度为2048，占据一个块)AACC(确保串不被一次清空)
  1
  2048 B
  ```
