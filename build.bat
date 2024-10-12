g++ -DUNICODE -I./imgui -I./back main.cpp ./bmpfunc.cpp ./back/imgui_impl_opengl3.cpp ./back/imgui_impl_win32.cpp ./imgui/imgui*.cpp -lopengl32 -lgdi32 -ldwmapi -o run.exe
@rem -DUNICODE "-I./imgui" "-I./back" main.cpp "./back/imgui_impl_opengl3.cpp" "./back/imgui_impl_win32.cpp" "./imgui/imgui*.cpp" "-lopengl32" "-lgdi32" "-ldwmapi" -o run.exebuild.bat
