# Coxide
Consider this as a homebrew solution to other stuff because I haven't done enough testing, if there is any bug please report

### Building
```sh
make
```


### Installing
```sh
make install
```

### Usage
Move the `o` function from shellrc/coxide.\* to your .zshrc/.bashrc (and rename it from `o` to anything(maybe even `cd`)) and start using o as regular cd or enter invalid directory path to automatically cd to closest path recorded
```sh
o ../../etc/xdg
```

```sh
o dg # automatically cds to /etc/xdg
```




