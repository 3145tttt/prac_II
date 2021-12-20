int
satsum(int v1, int v2)
{
    const int MAX_POS = (~0u) >> (!0), MAX_NEG = ~MAX_POS;
    if (v2 > 0 && v1 > 0 && (v1 > MAX_POS - v2))
        return MAX_POS;
    if (v2 < 0 && v1 < 0 && (v1 < MAX_NEG - v2))
        return MAX_NEG;
    return v1 + v2;
}