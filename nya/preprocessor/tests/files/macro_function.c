#define helper(x) \
    {             \
        x = 2;    \
    }

int main()
{
    int inta = 3;
    helper(inta);
    inta = inta + 3;
}