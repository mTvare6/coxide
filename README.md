# Coxide

coxide keeps track of the directories you used frequently and change directory using a shell function(reason explained later)

This is in very alpha stage because I haven't done enough testing, if there is any bug please report

### Building
```sh
make
```


### Installing
```sh
make install
```

### Usage
Run `eval `
```sh
o ../../etc/xdg
```

```sh
o dg # automatically cds to /etc/xdg
```

```sh
o - # alias to popd
```

### Adding the o alias to chdir

There is no api exposed by posix compliant shells to change directory, because no program should change the directory of the shell and it is done by a shell built in `cd` coxide uses `cd` in a function to work by sourcing a function which runs cd on current shell instance

<details>
<summary><code>​</code></summary>
Unlike zoxide which claim to be a 
> blazing fast(🚀) replacement for your cd command
(which actually depending on cd)

To start using `coxide`, add it to your shell.
</details>

##### Bash
Add this to your configuration (usually `~/.bashrc`):

```sh
eval "$(coxide shell bash)"
```

##### Elvish(not tested)
Add this to your configuration (usually `~/.elvish/rc.elv`):

```sh
eval (coxide shell elvish | slurp)
```

##### Fish
Add this to your configuration (usually `~/.config/fish/config.fish`):

```fish
coxide init fish | source
```

##### Powershell(not tested)
Add this to your configuration (find it by running `echo $profile` in PowerShell):

```powershell
Invoke-Expression (& {
    $hook = if ($PSVersionTable.PSVersion.Major -lt 6) { 'prompt' } else { 'pwd' }
    (coxide init --hook $hook powershell) -join "`n"
})
```

##### xonsh(not tested)
Add this to your configuration (usually `~/.xonshrc`):

```python
execx($(coxide shell xonsh), 'exec', __xonsh__.ctx, filename='coxide')
```


##### zsh
Add this to your configuration (usually `~/.zshrc`):

```sh
eval "$(coxide shell zsh)"
```


##### most posix shells(tested on dash)
Add this to your configuration:

```sh
eval "$(coxide shell sh)"
```





