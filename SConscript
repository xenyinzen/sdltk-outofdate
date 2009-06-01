

Import('env')
env.Append(CCFLAGS = '-Isrc/ui/sdltk/')
env.Append(CCFLAGS = '-Isrc')
env.Append(CCFLAGS = '-I/usr/include/libxml2')
env.Append(CCFLAGS = '-I/usr/include/SDL')
env.Append(CCFLAGS = '-I/usr/include/freetype2')
env.Append(CCFLAGS = '-ID:/sdltk1.0.4/sdltk1.0.4/include/freetype2')
env.Append(CCFLAGS = '-ID:\sdltk1.0.4\sdltk1.0.4\include')
env.Append(CCFLAGS = '-ID:\sdltk1.0.4\sdltk1.0.4\sdltk1.0.4')
env.Append(CCFLAGS = '-I../freetype2')
env.Append(CCFLAGS = '-Wall')
env.Append(CCFLAGS = '-g')
env.Append(CCFLAGS = '-ansi')
env.Append(CCFLAGS = '-pg')
env.Append(LINKFLAGS = '-pg')

sdltk = env.SharedLibrary('sdltk',
                          ['SDL_Button.c',
                           'SDL_DefFont.c',
                           'SDL_Edit.c',
                           'SDL_Font.c',
                           'SDL_FontBDF.c',
                           'SDL_FontFNT.c',
                           'SDL_FontTTF.c',
                           'SDL_Frame.c',
#                           'SDL_GLPrimitives.c',
                           'SDL_Images.c',
                           'SDL_ImageGIF.c',
                           'SDL_ImageFrame.c',
                           'SDL_Knob.c',
                           'SDL_Label.c',
                           'SDL_LList.c',
                           'SDL_Menu.c',
                           'SDL_MenuItem.c',
                           'SDL_Model.c',
                           'SDL_ModelType.c',
                           'SDL_MultiLineEdit.c',
                           'SDL_Panel.c',
                           'SDL_Paragraph.c',
                           'SDL_Primitives.c',
                           'SDL_ProgressBar.c',
                           'SDL_RadioButton.c',
                           'SDL_ScrollArea.c',
                           'SDL_Scrollbar.c',
                           'SDL_SelectionModel.c',
                           'SDL_Signal.c',
                           'SDL_Skin.c',
                           'SDL_SkinStyle.c',
                           'SDL_SkinWidget.c',
                           'SDL_SkinWindow.c',
                           'SDL_Slider.c',
                           'SDL_String.c',
                           'SDL_Style.c',
                           'SDL_StyleDefault.c',
                           'SDL_StyleXP.c',
                           'SDL_Tab.c',
                           'SDL_Table.c',
                           'SDL_TableCell.c',
                           'SDL_TableRow.c',
                           'SDL_ToggleButton.c',
                           'SDL_Tooltip.c',
                           'SDL_Tree.c',                                
                           'SDL_TreeModel.c',                                
                           'SDL_ViewItem.c',
                           'SDL_VolumeBar.c',
                           'SDL_Widget.c',
                           'SDL_WidgetType.c',
                           'SDL_WidTool.c',
                           'SDL_Window.c',
                           'SDL_WindowManager.c',
                           'SDL_WindowPrimitives.c',
                           'SDLTk.c',
                           'SDLTk_libc.c'])

Export('sdltk')




env.Alias(target = "install", source=env.Install(dir="/mnt/img/lib/",source="libsdltk.so"))
env.Alias(target = "install", source=env.Install(dir="/mnt/img/include/SDL",source=
                          ['SDL_Button.h',
                           'SDL_Color.h',
                           'SDL_Edit.h',
                           'SDL_Font.h',
                           'SDL_FontBDF.h',
                           'SDL_FontFNT.h',
                           'SDL_FontTTF.h',
                           'SDL_Frame.h',
#                           'SDL_GLPrimitives.h',
                           'SDL_Images.h',
                           'SDL_ImageGIF.h',
                           'SDL_ImageFrame.h',
                           'SDL_Knob.h',
                           'SDL_Label.h',
                           'SDL_LList.h',
                           'SDL_Menu.h',
                           'SDL_MenuItem.h',
                           'SDL_Model.h',
                           'SDL_ModelType.h',
                           'SDL_MultiLineEdit.h',
                           'SDL_Panel.h',
                           'SDL_Paragraph.h',
                           'SDL_Primitives.h',
                           'SDL_ProgressBar.h',
                           'SDL_RadioButton.h',
                           'SDL_ScrollArea.h',
                           'SDL_Scrollbar.h',
                           'SDL_SelectionModel.h',
                           'SDL_Signal.h',
                           'SDL_Skin.h',
                           'SDL_SkinStyle.h',
                           'SDL_SkinWidget.h',
                           'SDL_SkinWindow.h',
                           'SDL_Slider.h',
                           'SDL_String.h',
                           'SDL_Style.h',
                           'SDL_StyleDefault.h',
                           'SDL_StyleXP.h',
                           'SDL_Tab.h',
                           'SDL_Table.h',
                           'SDL_TableCell.h',
                           'SDL_TableRow.h',
                           'SDL_ToggleButton.h',
                           'SDL_Tooltip.h',
                           'SDL_Tree.h',                                
                           'SDL_TreeModel.h',                                
                           'SDL_ViewItem.h',
                           'SDL_VolumeBar.h',
                           'SDL_Widget.h',
                           'SDL_WidgetType.h',
                           'SDL_WidTool.h',
                           'SDL_Window.h',
                           'SDL_WindowManager.h',
                           'SDL_WindowPrimitives.h',
                           'SDLTk.h',
                           'SDLTk_libc.h']))









