#include <cstdio>
#include <bits/stdc++.h>
#include <time.h>
#include "togasat.hpp"
using namespace std;
int iteration = 1;
int literal_no = 33;
const char color[9][10] = {" -1 ", "RED", "GREEN", "BLUE", "YELLOW", "ORANGE", "WHITE", "BLACK", "BROWN"};
vector<vector<int>> clauses;
vector<int> code;
vector<int> guess(5);
void (*B_W_[5][5])();
int B = 0, W = 0;

int location(int i, int j)
{
    return (i - 1) * 8 + j;
}

void add_clause(vector<vector<int>> temp)
{
    for (int i = 0; i < temp.size(); i++)
    {
        clauses.push_back(temp[i]);
    }
}

vector<vector<int>> Bi_directional_with_CNF(int ln, vector<int> CNF)
{
    //To Bi-directionally imply the provided CNF with a literal
    vector<vector<int>> result;
    vector<int> temp;
    for (int i = 0; i < CNF.size(); i++)
    {
        temp.clear();
        temp.push_back(-ln);
        temp.push_back(CNF[i]);
        result.push_back(temp);
    }
    temp.clear();
    for (int i = 0; i < CNF.size(); i++)
    {
        temp.push_back(-CNF[i]);
    }
    temp.push_back(ln);
    result.push_back(temp);
    return result;
}

vector<vector<int>> Bi_directional_with_DNF(int ln, vector<int> DNF)
{
    //To Bi-directionally imply the provided DNF with a literal
    vector<vector<int>> result;
    vector<int> temp;
    temp.push_back(-ln);
    for (int i = 0; i < DNF.size(); i++)
    {
        temp.push_back(DNF[i]);
    }
    result.push_back(temp);
    for (int i = 0; i < DNF.size(); i++)
    {
        temp.clear();
        temp.push_back(-DNF[i]);
        temp.push_back(ln);
        result.push_back(temp);
    }
    return result;
}

void B_0_W_0()
{
    //Each geuss[i] i.e color should never appear again at any position.
    vector<int> vec;
    for (int i = 1; i <= 4; i++) //color
    {
        for (int j = 1; j <= 4; j++) //Position
        {
            vec.push_back(-location(j, guess[i]));
        }
    }
    int t1 = literal_no++;
    vector<vector<int>> temp = Bi_directional_with_CNF(t1, vec);
    add_clause(temp);

    clauses.push_back(vector<int>{t1});
}

void B_0_W_1()
{
    //Exactly One color is correct but no Color+position is correct
    for (int i = 1; i <= 4; i++) //No Color+position is correct
    {
        int pos = -location(i, guess[i]);
        clauses.push_back(vector<int>{pos});
    }
    //One color will repeat again in any position other than current position.
    vector<int> vec;
    vector<vector<int>> temp;
    int x[5] = {};
    for (int i = 1; i <= 4; i++) //color
    {
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (i != j)
            {
                vec.push_back(location(j, guess[i]));
            }
        }
        int t1 = literal_no++;
        temp = Bi_directional_with_DNF(t1, vec);
        add_clause(temp);

        vec.clear();
        for (int j = 1; j <= 4; j++) //other colors
        {
            if (j != i)
            {
                for (int k = 1; k <= 4; k++)
                {
                    vec.push_back(-location(k, guess[j]));
                }
            }
        }
        vec.push_back(t1);
        int t2 = literal_no++;
        temp = Bi_directional_with_CNF(t2, vec);
        add_clause(temp);

        x[i] = t2;
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    int t3 = literal_no++;
    temp = Bi_directional_with_DNF(t3, vec);
    add_clause(temp);

    //x[1] V x[2] V x[3] V x[4] <-->t3
    clauses.push_back(vector<int>{t3});
}

void B_0_W_2()
{
    //Exactly two colors are correct and no color+position is correct
    for (int i = 1; i <= 4; i++) //No color+position is correct.
    {
        int pos = -location(i, guess[i]);
        clauses.push_back(vector<int>{pos});
    }
    vector<int> vec;
    vector<vector<int>> temp;
    //Exactly two color will be repeated other than at their current position.
    int x[5] = {};
    for (int i = 1; i <= 3; i++) //Color for 1st w
    {
        int y[5] = {};
        for (int j = i + 1; j <= 4; j++) //color for 2nd W
        {
            //color for 2nd W will repeat at other position.
            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (k != j)
                {
                    vec.push_back(location(k, guess[j]));
                }
            }
            int t1 = literal_no++;
            temp = Bi_directional_with_DNF(t1, vec);
            add_clause(temp);

            //other color will never appear again at any position.
            int r1, r2;
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j)
                {
                    r1 = k;
                    break;
                }
            }
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j && k != r1)
                {
                    r2 = k;
                    break;
                }
            }

            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                vec.push_back(-location(k, guess[r1]));
                vec.push_back(-location(k, guess[r2]));
            }
            vec.push_back(t1);
            int t2 = literal_no++;
            temp = Bi_directional_with_CNF(t2, vec);
            add_clause(temp);

            y[j] = t2;
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        int t3 = literal_no++;
        temp = Bi_directional_with_DNF(t3, vec);
        add_clause(temp);

        //Color for 1st W will appear again at any other position.
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (i != j)
            {
                vec.push_back(location(j, guess[i]));
            }
        }
        int t4 = literal_no++;
        temp = Bi_directional_with_DNF(t4, vec);
        add_clause(temp);

        // t3 and t4 <--> t5
        vec.clear();
        vec.push_back(t3);
        vec.push_back(t4);
        int t5 = literal_no++;
        temp = Bi_directional_with_CNF(t5, vec);
        add_clause(temp);

        x[i] = t5;
    }

    //x[1] V x[2] V x[3] <--> t6
    vec.clear();
    for (int i = 1; i <= 3; i++)
    {
        vec.push_back(x[i]);
    }
    int t6 = literal_no++;
    temp = Bi_directional_with_DNF(t6, vec);
    add_clause(temp);

    clauses.push_back(vector<int>{t6});
}

void B_0_W_3()
{
    //Exactly three colors are correct and no color+position is correct
    for (int i = 1; i <= 4; i++) //No color+position is correct.
    {
        int pos = -location(i, guess[i]);
        clauses.push_back(vector<int>{pos});
    }
    vector<int> vec;
    vector<vector<int>> temp;
    int x[5] = {};
    for (int i = 1; i <= 4; i++)
    {
        //rest of the color will appear again except at the current position
        int y[5] = {};
        for (int j = 1; j <= 4; j++) //color
        {
            if (i == j)
                continue;
            vec.clear();
            for (int k = 1; k <= 4; k++) //position
            {
                if (k != j)
                {
                    vec.push_back(location(k, guess[j]));
                }
            }
            int t1 = literal_no++;
            temp = Bi_directional_with_DNF(t1, vec);
            add_clause(temp);
            y[j] = t1;
        }

        //ith color is not for W
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            vec.push_back(-location(j, guess[i]));
        }
        for (int j = 1; j <= 4; j++) //do not clear vector
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        int t2 = literal_no++;
        temp = Bi_directional_with_CNF(t2, vec);
        add_clause(temp);

        x[i] = t2;
    }

    //x[1] V x[2] V x[3] V x[4] <--> t3
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }

    int t3 = literal_no++;
    temp = Bi_directional_with_DNF(t3, vec);
    add_clause(temp);

    clauses.push_back(vector<int>{t3});
}

void B_0_W_4()
{
    //All these color will repeat and no color+position is correct
    for (int i = 1; i <= 4; i++) //All color+pos is incorrect.
    {
        int pos = -location(i, guess[i]);
        clauses.push_back(vector<int>{pos});
    }
    vector<int> vec;
    vector<vector<int>> temp;
    int x[5] = {};
    for (int i = 1; i <= 4; i++) //color
    {
        vec.clear();
        for (int j = 1; j <= 4; j++) //position
        {
            if (i != j)
            {
                vec.push_back(location(j, guess[i]));
            }
        }
        x[i] = literal_no++;
        temp = Bi_directional_with_DNF(x[i], vec);
        add_clause(temp);
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }

    //x1 and x2 and x3 and x4 <-->t2
    int t2 = literal_no++;
    temp = Bi_directional_with_CNF(t2, vec);
    add_clause(temp);

    clauses.push_back(vector<int>{t2});
}

void B_1_W_0()
{
    //Exactly one color+pos is correct and others are not at color+pos
    int x[5] = {};
    vector<int> vec;
    vector<vector<int>> temp;
    for (int i = 1; i <= 4; i++) //B term color
    {
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (i == j) //same color
                continue;
            for (int k = 1; k <= 4; k++) //at any position
            {
                vec.push_back(-location(k, guess[j]));
            }
        }
        vec.push_back(location(i, guess[i]));
        x[i] = literal_no++;
        temp = Bi_directional_with_CNF(x[i], vec);
        add_clause(temp);
    }

    //x[1] V x[2] V x[3] V x[4] <--> z
    int z = literal_no++;
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    temp = Bi_directional_with_DNF(z, vec);
    add_clause(temp);

    clauses.push_back(vector<int>{z});
}

void B_1_W_1()
{
    //one is at correct color+position and one is at correct color
    int x[5] = {};
    vector<int> vec;
    vector<vector<int>> temp;
    for (int i = 1; i <= 4; i++) //B term
    {
        int y[5] = {};
        vec.clear();
        for (int j = 1; j <= 4; j++) //W term
        {
            if (i != j)
            {
                vec.clear();
                for (int k = 1; k <= 4; k++)
                {
                    if (k != i && k != j)
                    {
                        vec.push_back(location(k, guess[j]));
                    }
                }
                int t1 = literal_no++;
                temp = Bi_directional_with_DNF(t1, vec);
                add_clause(temp);

                vec.clear();
                for (int k = 1; k <= 4; k++) //all other colors will never appear again ever
                {
                    if (k != i && k != j)
                    {
                        for (int l = 1; l <= 4; l++)
                        {
                            vec.push_back(-location(l, guess[k]));
                        }
                    }
                }
                vec.push_back(t1);
                vec.push_back(-location(j, guess[j]));
                int t2 = literal_no++;
                temp = Bi_directional_with_CNF(t2, vec);
                add_clause(temp);
                y[j] = t2;
            }
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        int t3 = literal_no++;
        temp = Bi_directional_with_DNF(t3, vec);
        add_clause(temp);

        vec.clear();
        vec.push_back(location(i, guess[i]));
        vec.push_back(t3);
        int t4 = literal_no++;
        temp = Bi_directional_with_CNF(t4, vec);
        add_clause(temp);
        x[i] = t4;
    }
    //x1 V x2 x3 Vx4 <--> z
    int z = literal_no++;
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    temp = Bi_directional_with_DNF(z, vec);
    add_clause(temp);
    clauses.push_back(vector<int>{z});
}
void B_1_W_2()
{
    //one is at correct color+position and two of them from rest are correct color.
    int x[5] = {};
    vector<int> vec;
    vector<vector<int>> temp;
    for (int i = 1; i <= 4; i++) //B term
    {
        int y[5] = {};
        for (int j = 1; j <= 4; j++) //W1 term
        {
            int z[5] = {};
            if (i != j)
            {
                for (int k = j + 1; k <= 4; k++) //w2 term
                {
                    if (k != i && k != j)
                    {
                        vec.clear();
                        for (int l = 1; l <= 4; l++)
                        {
                            if (l != i && l != k)
                                vec.push_back(location(l, guess[k]));
                        }
                        int t1 = literal_no++;
                        temp = Bi_directional_with_DNF(t1, vec);
                        add_clause(temp);

                        int rem = 0;
                        for (int l = 1; l <= 4; l++) //remaining one color will never again appear
                        {
                            if (l != i && l != j && l != k)
                                rem = l;
                        }
                        vec.clear();
                        for (int l = 1; l <= 4; l++)
                        {
                            vec.push_back(-location(l, guess[rem]));
                        }
                        int t2 = literal_no++;
                        temp = Bi_directional_with_CNF(t2, vec);
                        add_clause(temp);

                        int t3 = literal_no++;
                        temp = Bi_directional_with_CNF(t3, vector<int>{t1, t2, -location(k, guess[k])});
                        add_clause(temp);

                        z[k] = t3;
                    }
                }
                int t4 = literal_no++;
                vec.clear();
                for (int l = 1; l <= 4; l++)
                {
                    if (z[l] != 0)
                    {
                        vec.push_back(z[l]);
                    }
                }
                temp = Bi_directional_with_DNF(t4, vec);
                add_clause(temp);

                int t5 = literal_no++;
                vec.clear();
                vec.push_back(t4);
                vec.push_back(-location(j, guess[j]));
                temp = Bi_directional_with_CNF(t5, vec);
                add_clause(temp);

                y[j] = t5;
            }
        }
        vec.clear();
        for (int l = 1; l <= 4; l++)
        {
            if (y[l] != 0)
                vec.push_back(y[l]);
        }
        int t6 = literal_no++;
        temp = Bi_directional_with_DNF(t6, vec);
        add_clause(temp);

        int t7 = literal_no++;
        vec.clear();
        vec.push_back(t6);
        vec.push_back(location(i, guess[i]));
        temp = Bi_directional_with_CNF(t7, vec);
        add_clause(temp);

        x[i] = t7;
    }

    vec.clear();
    for (int j = 1; j <= 4; j++)
    {
        if (x[j] != 0)
        {
            vec.push_back(x[j]);
        }
    }
    int t8 = literal_no++;
    temp = Bi_directional_with_DNF(t8, vec);
    add_clause(temp);
    clauses.push_back(vector<int>{t8});
    //x[1] V x[2] V x[3] V x[4] <-->t8
}
void B_1_W_3()
{
    //one is at correct color+position and the rest are correct color
    int x[5] = {};
    vector<int> vec;
    vector<vector<int>> temp;
    for (int i = 1; i <= 4; i++)
    {
        vec.clear();
        for (int j = 1; j <= 4; j++) //rest are not at correct position
        {
            if (i != j)
            {
                vec.push_back(-location(j, guess[j]));
            }
        }
        int t1 = literal_no++;
        temp = Bi_directional_with_CNF(t1, vec);
        add_clause(temp);

        int y[5] = {};
        for (int j = 1; j <= 4; j++)
        {
            if (j == i)
                continue;
            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j)
                {
                    vec.push_back(location(k, guess[j]));
                }
            }
            int t2 = literal_no++;
            temp = Bi_directional_with_DNF(t2, vec);
            add_clause(temp);
            y[j] = t2;
        }

        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        vec.push_back(t1);
        vec.push_back(location(i, guess[i]));
        int t3 = literal_no++;
        temp = Bi_directional_with_CNF(t3, vec);
        add_clause(temp);
        x[i] = t3;
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        if (x[i] != 0)
            vec.push_back(x[i]);
    }
    int t4 = literal_no++;
    temp = Bi_directional_with_DNF(t4, vec);
    add_clause(temp);
    clauses.push_back(vector<int>{t4});
    //x[1] v x[2] v x[3] v x[4] <--> t4
}
void B_2_W_0()
{
    //two of the colors are at the correct position
    int x[5] = {};
    vector<int> vec;
    vector<vector<int>> temp;
    for (int i = 1; i <= 3; i++) //x terms or B1 term
    {
        int y[5] = {};
        for (int j = i + 1; j <= 4; j++) //y term or B2 term
        {
            int r1, r2;
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j)
                {
                    r1 = k;
                    break;
                }
            }
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j && k != r1)
                {
                    r2 = k;
                    break;
                }
            }
            vec.clear();
            for (int k = 1; k <= 4; k++) //rest of the color will never appear again
            {
                vec.push_back(-location(k, guess[r1]));
                vec.push_back(-location(k, guess[r2]));
            }
            vec.push_back(location(j, guess[j]));
            int t1 = literal_no++;
            temp = Bi_directional_with_CNF(t1, vec);
            add_clause(temp);

            y[j] = t1;
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        int t2 = literal_no++;
        temp = Bi_directional_with_DNF(t2, vec);
        add_clause(temp);

        vec.clear();
        vec.push_back(t2);
        vec.push_back(location(i, guess[i]));
        int t3 = literal_no++;
        temp = Bi_directional_with_CNF(t3, vec);
        add_clause(temp);

        x[i] = t3;
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        if (x[i] != 0)
        {
            vec.push_back(x[i]);
        }
    }
    int t4 = literal_no++;
    temp = Bi_directional_with_DNF(t4, vec);
    add_clause(temp);
    clauses.push_back(vector<int>{t4});
    //x[1] V x[2] V x[3] V x[4] <-->t4
}
void B_2_W_1()
{
    //two colors at the correct position and one is correct color but not position
    vector<int> vec;
    vector<vector<int>> temp;
    int x[5] = {};
    for (int i = 1; i <= 3; i++) //x term or B1 term
    {
        int y[5] = {};
        for (int j = i + 1; j <= 4; j++) //y term or B2 term
        {
            int r1, r2;
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j)
                {
                    r1 = k;
                    break;
                }
            }
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j && k != r1)
                {
                    r2 = k;
                    break;
                }
            }
            vec.clear();
            vec.push_back(location(r1, guess[r2]));
            vec.push_back(location(r2, guess[r1]));
            int t1 = literal_no++;
            temp = Bi_directional_with_DNF(t1, vec);
            add_clause(temp);

            vec.clear();
            vec.push_back(t1);
            vec.push_back(location(j, guess[j]));
            int t2 = literal_no++;
            temp = Bi_directional_with_CNF(t2, vec);
            add_clause(temp);

            y[j] = t2;
        }
        vec.clear();
        int t3 = literal_no++;
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        temp = Bi_directional_with_DNF(t3, vec);
        add_clause(temp);

        vec.clear();
        vec.push_back(t3);
        vec.push_back(location(i, guess[i]));
        int t4 = literal_no++;
        temp = Bi_directional_with_CNF(t4, vec);
        add_clause(temp);
        x[i] = t4;
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        if (x[i] != 0)
        {
            vec.push_back(x[i]);
        }
    }
    int t5 = literal_no++;
    temp = Bi_directional_with_DNF(t5, vec);
    add_clause(temp);

    clauses.push_back(vector<int>{t5});
    //x[1] V x[2] V x[3] V x[4] <--> t5
}
void B_2_W_2()
{
    //two of the colors are at correct position and the rest of the color are at each other positions.
    vector<int> vec;
    vector<vector<int>> temp;
    int x[5] = {};
    for (int i = 1; i <= 3; i++) //B1 term
    {
        int y[5] = {};
        for (int j = i + 1; j <= 4; j++) //B2 term
        {
            int r1, r2;
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j)
                {
                    r1 = k;
                    break;
                }
            }
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j && k != r1)
                {
                    r2 = k;
                    break;
                }
            }
            vec.clear();
            vec.push_back(location(j, guess[j]));
            vec.push_back(location(r1, guess[r2]));
            vec.push_back(location(r2, guess[r1]));

            int t1 = literal_no++;
            temp = Bi_directional_with_CNF(t1, vec);
            add_clause(temp);

            y[j] = t1;
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        int t2 = literal_no++;
        temp = Bi_directional_with_DNF(t2, vec);
        add_clause(temp);

        vec.clear();
        vec.push_back(t2);
        vec.push_back(location(i, guess[i]));
        int t3 = literal_no++;
        temp = Bi_directional_with_CNF(t3, vec);
        add_clause(temp);

        x[i] = t3;
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        if (x[i] != 0)
        {
            vec.push_back(x[i]);
        }
    }
    int t4 = literal_no++;
    temp = Bi_directional_with_DNF(t4, vec);
    add_clause(temp);

    clauses.push_back(vector<int>{t4});
    //x[1] V x[2] V x[3] V x[4] <-->t4
}
void B_3_W_0()
{
    //three of the colors are at the correct position
    vector<int> vec;
    vector<vector<int>> temp;
    int x[5] = {};
    for (int i = 1; i <= 4; i++)
    {
        vec.clear();
        for (int j = 1; j <= 4; j++) //W term   i.e remaining color will never appear again.
        {
            vec.push_back(-location(j, guess[i]));
        }
        for (int j = 1; j <= 4; j++) //B term
        {
            if (i != j)
            {
                vec.push_back(location(j, guess[j]));
            }
        }
        int t1 = literal_no++;
        temp = Bi_directional_with_CNF(t1, vec);
        add_clause(temp);

        x[i] = t1;
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    int t2 = literal_no++;
    temp = Bi_directional_with_DNF(t2, vec);
    add_clause(temp);

    clauses.push_back(vector<int>{t2});
    //x[1] V x[2] V x[3] V x[4] <--> t2
}
void feedback()
{
    //provides feedback for the guessed colors.
    B = 0;
    W = 0;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            if (code[i] == guess[j])
            {
                if (i == j)
                    B++;
                else
                    W++;
                break;
            }
        }
    }
    cout << "\t\tB = " << B << " W = " << W << endl
         << endl;
}
void make_code()
{
    //generate the code of codemaker.
    srand(time(0));
    bool colors_choosen[9] = {};
    int i = 1;
    code.push_back(-1);
    while (i <= 4)
    {
        int c = rand() % 8 + 1;
        if (!colors_choosen[c])
        {
            colors_choosen[c] = 1;
            code.push_back(c);
            //cout << color[code[i]] << " | ";
            i++;
        }
    }
    cout << "CODE--------------";
    for (int i = 1; i <= 4; i++)
    {
        cout << color[code[i]] << " ******************";
    }
    cout << endl
         << endl;
}
void ADD_initial_constraints()
{
    //Adding initial Game rules and related constraints
    vector<int> clause;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 8; j++)
        {

            int antecedant = -location(i, j);
            for (int i1 = 1; i1 <= 8; i1++)
            {
                if (i1 != j)
                {
                    clause.clear();
                    int consequent = -location(i, i1);
                    clause.push_back(antecedant);
                    clause.push_back(consequent);
                    clauses.push_back(clause);
                }
            }
            for (int i1 = 1; i1 <= 4; i1++)
            {
                if (i1 != i)
                {
                    clause.clear();
                    int consequent = -location(i1, j);
                    clause.push_back(antecedant);
                    clause.push_back(consequent);
                    clauses.push_back(clause);
                }
            }
        }
    }
    for (int i = 1; i <= 4; i++)
    {
        clause.clear();
        for (int j = 1; j <= 8; j++)
        {
            clause.push_back(location(i, j));
        }
        clauses.push_back(clause);
    }
}

void Code_Breaker()
{
    //Code_breaker function
    togasat::Solver solver;
    for (int i = 0; i < clauses.size(); i++)
    {
        solver.addClause(clauses[i]);
    }
    solver.solve();
    int j = 1;
    for (int i = 0; i < 32; i++)
    {
        if (solver.assigns[i] == 0)
        {
            int g = i + 1;
            guess[j] = g - (ceil(g / 8.0) - 1) * 8;
            j++;
        }
    }
    cout << "Iteration: " << iteration++ << "------";
    for (int i = 1; i <= 4; i++)
    {
        cout << color[guess[i]] << " -----------------";
    }
}

void pair_functions()
{
    //paring constraint adding functions with appropriate feedback.
    B_W_[0][0] = &B_0_W_0;
    B_W_[0][1] = &B_0_W_1;
    B_W_[0][2] = &B_0_W_2;
    B_W_[0][3] = &B_0_W_3;
    B_W_[0][4] = &B_0_W_4;
    B_W_[1][0] = &B_1_W_0;
    B_W_[1][1] = &B_1_W_1;
    B_W_[1][2] = &B_1_W_2;
    B_W_[1][3] = &B_1_W_3;
    B_W_[2][0] = &B_2_W_0;
    B_W_[2][1] = &B_2_W_1;
    B_W_[2][2] = &B_2_W_2;
    B_W_[3][0] = &B_3_W_0;
}

int main()
{
    make_code();

    ADD_initial_constraints();

    pair_functions();
    do
    {
        Code_Breaker();
        feedback();
        if (B == 4)
            break;
        B_W_[B][W]();       //Add constraints based on feedback if All Balls are not correctly placed
    } while (B != 4);
    cout << endl;
    return 0;
}
