mkdir -p  ubin
cc advShell.c -o ubin/advShell
cc createMenu.c -o ubin/createMenu
cc getPrice.c -o ubin/getPrice
chmod +x ubin/advShell
chmod +x ubin/createMenu
chmod +x ubin/getPrice
echo "./ubin/advShell" >> ~/.bash_history
./ubin/advShell