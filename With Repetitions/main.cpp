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

void Bi_directional_with_CNF(int ln, vector<int> CNF)
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
    add_clause(result);
}

void Bi_directional_with_DNF(int ln, vector<int> DNF)
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
    add_clause(result);
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
    Bi_directional_with_CNF(t1, vec);

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
        Bi_directional_with_DNF(t1, vec);

        x[i] = t1;
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    int t3 = literal_no++;
    Bi_directional_with_DNF(t3, vec);

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
            Bi_directional_with_DNF(t1, vec);

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
        int t3 = literal_no++;
        Bi_directional_with_DNF(t3, vec);

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
        Bi_directional_with_DNF(t4, vec);

        // t3 and t4 <--> t5
        vec.clear();
        vec.push_back(t3);
        vec.push_back(t4);
        int t5 = literal_no++;
        Bi_directional_with_CNF(t5, vec);

        x[i] = t5;
    }

    //x[1] V x[2] V x[3] <--> t6
    vec.clear();
    for (int i = 1; i <= 3; i++)
    {
        vec.push_back(x[i]);
    }
    int t6 = literal_no++;
    Bi_directional_with_DNF(t6, vec);

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

    //Three color will definitely repeat again at any other position, 4th color may repeat again.
    int x[3] = {};
    for (int i = 1; i <= 2; i++) //1st W
    {
        int y[5] = {};
        for (int j = i + 1; j <= 3; j++) //2nd W
        {
            int z[5] = {};
            for (int k = j + 1; k <= 4; k++) //3rd W
            {
                vec.clear();
                for (int l = 1; l <= 4; l++)
                {
                    if (k != l)
                    {
                        vec.push_back(location(l, guess[k]));
                    }
                }
                int t1 = literal_no++;
                Bi_directional_with_DNF(t1, vec);
                z[k] = t1;
            }
            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (z[k] != 0)
                {
                    vec.push_back(z[k]);
                }
            }
            int t2 = literal_no++;
            Bi_directional_with_DNF(t2, vec);

            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (k != j)
                {
                    vec.push_back(location(k, guess[j]));
                }
            }
            int t3 = literal_no++;
            Bi_directional_with_DNF(t3, vec);

            vec.clear();
            vec.push_back(t2);
            vec.push_back(t3);
            int t4 = literal_no++;
            Bi_directional_with_CNF(t4, vec);

            y[j] = t4;
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        int t5 = literal_no++;
        Bi_directional_with_DNF(t5, vec);

        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (i != j)
                vec.push_back(location(j, guess[i]));
        }
        int t6 = literal_no++;
        Bi_directional_with_DNF(t6, vec);

        vec.clear();
        vec.push_back(t5);
        vec.push_back(t6);
        int t7 = literal_no++;
        Bi_directional_with_CNF(t7, vec);

        x[i] = t7;
    }

    vec.clear();
    for (int i = 1; i <= 2; i++)
    {
        if (x[i] != 0)
        {
            vec.push_back(x[i]);
        }
    }
    int t8 = literal_no++;
    Bi_directional_with_DNF(t8, vec);

    clauses.push_back(vector<int>{t8});
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
        Bi_directional_with_DNF(x[i], vec);
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }

    //x1 and x2 and x3 and x4 <-->t2
    int t2 = literal_no++;
    Bi_directional_with_CNF(t2, vec);

    clauses.push_back(vector<int>{t2});
}

void B_1_W_0()
{
    //Exactly one color+pos is correct and others are not at color+pos
    vector<int> vec;
    int x[5] = {};
    for (int i = 1; i <= 4; i++) //B term color
    {
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (i != j)
            {
                vec.push_back(-location(j, guess[j]));
            }
        }
        vec.push_back(location(i, guess[i]));
        x[i] = literal_no++;
        Bi_directional_with_CNF(x[i], vec);
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    int t1 = literal_no++;
    Bi_directional_with_DNF(t1, vec);
    clauses.push_back(vector<int>{t1});
    //x[1] V x[2] V x[3] V x[4] <-->t1
}

void B_1_W_1()
{
    //one of the color is at correct position and one is the correct but not at the correct position
    int x[5] = {};
    vector<int> vec;
    for (int i = 1; i <= 4; i++) //for B
    {
        vec.clear();
        int y[5] = {};
        for (int j = 1; j <= 4; j++) //For W
        {
            if (i == j)
                continue;
            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j)
                {
                    vec.push_back(location(k, guess[j]));
                }
            }
            int t0 = literal_no++;
            Bi_directional_with_DNF(t0, vec);
            y[j] = t0;
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
                vec.push_back(y[j]);
        }
        int t1 = literal_no++;
        Bi_directional_with_DNF(t1, vec);

        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (i != j)
                vec.push_back(-location(j, guess[j]));
        }
        vec.push_back(t1);
        vec.push_back(location(i, guess[i]));
        int t2 = literal_no++;
        Bi_directional_with_CNF(t2, vec);

        x[i] = t2;
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    int t3 = literal_no++;
    Bi_directional_with_DNF(t3, vec);

    clauses.push_back(vector<int>{t3});
    //x[1] V x[2] V x[3] V x[4] <--> t3
}
void B_1_W_2()
{
    //one of the color is at the correct position and two of the remaining are the color are correct but not position.
    int x[5] = {};
    vector<int> vec;
    for (int i = 1; i <= 4; i++) //For B
    {
        int y[5] = {};
        for (int j = 1; j <= 3; j++) //For W1
        {
            if (i == j)
                continue;
            vec.clear();
            int z[5] = {};
            for (int k = j + 1; k <= 4; k++) //For W2
            {
                if (k == i)
                    continue;
                vec.clear();
                for (int l = 1; l <= 4; l++)
                {
                    if (l != i && l != k)
                    {
                        vec.push_back(location(l, guess[k]));
                    }
                }
                int t0 = literal_no++;
                Bi_directional_with_DNF(t0, vec);
                z[k] = t0;
            }
            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (z[k] != 0)
                {
                    vec.push_back(z[k]);
                }
            }
            int t1 = literal_no++;
            Bi_directional_with_DNF(t1, vec);

            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j)
                {
                    vec.push_back(location(k, guess[j]));
                }
            }
            int t2 = literal_no++;
            Bi_directional_with_DNF(t2, vec);

            vec.clear();
            vec.push_back(t2);
            vec.push_back(t1);
            int t3 = literal_no++;
            Bi_directional_with_CNF(t3, vec);

            y[j] = t3;
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        int t4 = literal_no++;
        Bi_directional_with_DNF(t4, vec);

        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (i != j)
            {
                vec.push_back(-location(j, guess[j]));
            }
        }
        vec.push_back(t4);
        vec.push_back(location(i, guess[i]));
        int t5 = literal_no++;
        Bi_directional_with_CNF(t5, vec);

        x[i] = t5;
    }

    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        if (x[i] != 0)
        {
            vec.push_back(x[i]);
        }
    }
    int t6 = literal_no++;
    Bi_directional_with_DNF(t6, vec);

    clauses.push_back(vector<int>{t6});
    //x[1] V x[2] V x[3] V x[4] <--> t6
}
void B_1_W_3()
{
    //one of the color is at the correct color+position and the remaining colors will appear again but not at their current position.
    int x[5] = {};
    vector<int> vec;
    for (int i = 1; i <= 4; i++)
    {
        int y[5] = {};
        for (int j = 1; j <= 4; j++)
        {
            if (i == j)
                continue;
            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j)
                {
                    vec.push_back(location(k, guess[j]));
                }
            }
            y[j] = literal_no++;
            Bi_directional_with_DNF(y[j], vec);
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
            {
                vec.push_back(y[j]);
            }
        }
        for (int j = 1; j <= 4; j++)
        {
            if (i != j)
            {
                vec.push_back(-location(j, guess[j]));
            }
        }
        vec.push_back(location(i, guess[i]));
        x[i] = literal_no++;
        Bi_directional_with_CNF(x[i], vec);
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    int t3 = literal_no++;
    Bi_directional_with_DNF(t3, vec);

    clauses.push_back(vector<int>{t3});
    //x[1] V x[2] V x[3] V x[4] <--> t3
}
void B_2_W_0()
{
    //Two of the colors are at their correct positiob
    int x[5] = {};
    vector<int> vec;
    for (int i = 0; i <= 3; i++) //for B1
    {
        vec.clear();
        int y[5] = {};
        for (int j = i + 1; j <= 4; j++) //for B2
        {
            vec.clear();
            for (int k = 1; k <= 4; k++)
            {
                if (k != i && k != j) //remained colors will not again appear at the same position.
                {
                    vec.push_back(-location(k, guess[k]));
                }
            }
            vec.push_back(location(j, guess[j]));
            y[j] = literal_no++;
            Bi_directional_with_CNF(y[j], vec);
        }
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (y[j] != 0)
                vec.push_back(y[j]);
        }
        int t1 = literal_no++;
        Bi_directional_with_DNF(t1, vec);

        vec.clear();
        vec.push_back(t1);
        vec.push_back(location(i, guess[i]));
        x[i] = literal_no++;
        Bi_directional_with_CNF(x[i], vec);
    }
    vec.clear();
    for (int i = 1; i <= 3; i++)
    {
        vec.push_back(x[i]);
    }
    int t2 = literal_no++;
    Bi_directional_with_DNF(t2, vec);
    clauses.push_back(vector<int>{t2});
    //x[1] V x[2] V x[3] V x[4] <--> t2
}
void B_2_W_1()
{
    //two of the colors are at their correct position and one of them will appear again
    vector<int> vec;
    int x[5] = {};
    for (int i = 1; i <= 3; i++) //For B1
    {
        int y[5] = {};
        for (int j = i + 1; j <= 4; j++) //For a B2 and any W
        {
            //remaining position
            int r1, r2;
            for (int i1 = 1; i <= 4; i1++)
            {
                if (i1 != i && i1 != j)
                {
                    r1 = i1;
                    break;
                }
            }
            for (int i1 = 1; i1 <= 4; i1++)
            {
                if (i1 != i && i1 != j && i1 != r1)
                {
                    r2 = i1;
                    break;
                }
            }

            vec.clear();
            vec.push_back(location(r1, guess[r2]));
            vec.push_back(location(r2, guess[r1]));
            int t1 = literal_no++;
            Bi_directional_with_DNF(t1, vec);

            vec.clear();
            vec.push_back(-location(r1, guess[r1]));
            vec.push_back(-location(r2, guess[r2]));

            vec.push_back(t1);
            vec.push_back(location(j, guess[j]));
            y[j] = literal_no++;
            Bi_directional_with_CNF(y[j], vec);
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
        Bi_directional_with_DNF(t3, vec);

        x[i] = literal_no++;
        vec.clear();
        vec.push_back(t3);
        vec.push_back(location(i, guess[i]));
        Bi_directional_with_CNF(x[i], vec);
    }
    vec.clear();
    for (int i = 1; i <= 3; i++)
    {
        vec.push_back(x[i]);
    }
    int t4 = literal_no++;
    Bi_directional_with_DNF(t4, vec);
    clauses.push_back(vector<int>{t4});
    //x[1] V x[2] V x[3] V x[4] <--> t4
}
void B_2_W_2()
{
    //two of the colors are at their correct position and rest will appear again.
    vector<int> vec;
    int x[5] = {};
    for (int i = 1; i <= 3; i++) //For B1
    {
        int y[5] = {};
        for (int j = i + 1; j <= 4; j++) //For a B2 and two W
        {
            //remaining position
            int r1, r2;
            for (int i1 = 1; i <= 4; i1++)
            {
                if (i1 != i && i1 != j)
                {
                    r1 = i1;
                    break;
                }
            }
            for (int i1 = 1; i1 <= 4; i1++)
            {
                if (i1 != i && i1 != j && i1 != r1)
                {
                    r2 = i1;
                    break;
                }
            }

            vec.clear();
            vec.push_back(location(r1, guess[r2]));
            vec.push_back(location(r2, guess[r1]));
            vec.push_back(-location(r1, guess[r1]));
            vec.push_back(-location(r2, guess[r2]));
            vec.push_back(location(j, guess[j]));
            y[j] = literal_no++;
            Bi_directional_with_CNF(y[j], vec);
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
        Bi_directional_with_DNF(t3, vec);

        x[i] = literal_no++;
        vec.clear();
        vec.push_back(t3);
        vec.push_back(location(i, guess[i]));
        Bi_directional_with_CNF(x[i], vec);
    }
    vec.clear();
    for (int i = 1; i <= 3; i++)
    {
        vec.push_back(x[i]);
    }
    int t4 = literal_no++;
    Bi_directional_with_DNF(t4, vec);
    clauses.push_back(vector<int>{t4});
    //x[1] V x[2] V x[3] V x[4] <--> t4
}
void B_3_W_0()
{
    //Three of the colors are at their correct position
    vector<int> vec;
    int x[5] = {};
    for (int i = 1; i <= 4; i++)
    {
        vec.clear();
        for (int j = 1; j <= 4; j++)
        {
            if (j != i)
            {
                vec.push_back(location(j, guess[j]));
            }
        }
        vec.push_back(-location(i, guess[i])); //remained one will not appear again at the same position.
        x[i] = literal_no++;
        Bi_directional_with_CNF(x[i], vec);
    }
    vec.clear();
    for (int i = 1; i <= 4; i++)
    {
        vec.push_back(x[i]);
    }
    int t1 = literal_no++;
    Bi_directional_with_DNF(t1, vec);
    clauses.push_back(vector<int>{t1});
    //x[1] V x[2] V x[3] V x[4] <--> t1
}
void feedback()
{
    //provides feedback for the guessed colors.
    B = 0;
    W = 0;
    vector<int> temp_g(guess);
    vector<int> temp_c(code);
    for (int i = 1; i <= 4; i++)
    {
        if (temp_c[i] == temp_g[i])
        {
            B++;
            temp_g[i] = -1;
            temp_c[i] = -2;
        }
    }
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            if (temp_c[i] == temp_g[j])
            {
                W++;
                temp_g[j] = -1;
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
    int i = 1;
    code.push_back(0);
    while (i <= 4)
    {
        int c = rand() % 8 + 1;
        code.push_back(c);
        i++;
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

void CodeBreaker()
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
        cout << color[guess[i]] << " --------------------";
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
        CodeBreaker();
        feedback();
        if (B == 4)
            break;
        B_W_[B][W](); //Add constraints based on feedback if All Balls are not correctly placed
    } while (B != 4);
    cout<<endl;
    return 0;
}
