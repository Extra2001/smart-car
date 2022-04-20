string prin = "";

for(int i = 1; i <=8; i++)
{
    var lines = File.ReadAllLines(i + ".txt").ToList();
    string print = "";
    double sum = 0;
    int cnt = 0;
    foreach (var item in lines)
    {
        int index = item.IndexOf("Z: ");
        string sub = item.Substring(index + 3);
        double z = double.Parse(sub);
        if (z != 0)
        {
            cnt++;
            print += z + "\n";
            sum += z;
        }
    }
    print += sum / cnt + "\n";
    prin += $"{i}: {sum / 33}\n";
    File.WriteAllText("out" + i + ".txt", print);
}
File.WriteAllText("avg.txt", prin);