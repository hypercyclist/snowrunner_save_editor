# SnowRunner - Save Editor.
# ![#f03c15]THE EDITOR NO LONGER WORKS. According to one user, IT CRASHES SAVES. So, don't use it.`#f03c15`

It is GUI desktop application for edit SnowRunner save file (PC only).
For now it supports only editing CompleteSave.cfg.

The main goal of application is editing exist save. 
For example your save was lost. You are create new one. And with this application edit your new created save file untill it looks like the old save file.

There are few functions:
Edit money count, rank and expiriece.
Edit difficulty settings.
Edit list of complete tasks. 
Edit list of founded upgrades.

All other things you can buy with created money.

App automatically translated in 13 languages with game files. There may be semantic errors here.

The app developed with C++, Qt, RapidJSON, RapidXML and ton of crutches.
It tested by me under Linux and Windows platforms with PC game version save files.

# IF APP BREAKS WITH ERROR, OR "none" WROTE EVERYWHERE, than try this:
The solution is to place the program with the rest of the files in the root of the disk, so that the path is something like this:
С:\snowrunner_save_editor\snowrunner_save_editor.exe
D:\snowrunner_save_editor\snowrunner_save_editor.exe
This should help.
Why does this happen - too long paths in the program folder structure. This is somehow related to the fact that the program is trying to read a file, and the path to it is longer than 256 characters and Windows cannot read it.

Always backup your save directory before editing save file. For example if the structure of save file will be changed this app can broke it. If one day your save will be broken with this app it's not my problem. Use on your own risks!

There are issues. For now tasks marked as completed isn't counted in statistics. Trucks upgrades too. I will fix it soon.
 
Free to use/edit/fork.
License: GPL.

Demo screenshot:

![](https://github.com/hypercyclist/snowrunner_save_editor/blob/main/images/demo_image_1.png)

![](https://github.com/hypercyclist/snowrunner_save_editor/blob/main/images/demo_image_2.png)

Release builds:

[Download release](https://github.com/hypercyclist/snowrunner_save_editor/releases)
