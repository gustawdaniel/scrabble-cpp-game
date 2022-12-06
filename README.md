```
sudo dnf install ncurses-devel
```

Font:

```
mkdir -p ~/.local/share/fonts
cp square.ttf ~/.local/share/fonts
fc-cache -f -v
fc-list | grep square
```