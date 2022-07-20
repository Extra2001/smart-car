var files = Directory.GetFiles(@"D:\成图");

int cnt = 0;
foreach (var item in files)
{
    cnt++;
    // 重命名为cnt.jpg
    File.Move(item, Path.Combine(@"D:\成图", cnt + ".jpg"));
}