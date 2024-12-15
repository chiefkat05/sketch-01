# all:
# 	# make pls

windows:
	x86_64-w64-mingw32-g++ ./sources/*.cpp -o main.exe -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lsfml-network -lws2_32 -lFLAC -lopenal32 -lvorbisenc -lvorbisfile -lvorbis -logg -lopengl32 -lfreetype -lwinmm -lgdi32 -static -O2


# target_link_libraries(main sfml-graphics sfml-window sfml-audio sfml-system
# sfml-network ws2_32 FLAC openal32 vorbisenc vorbisfile vorbis ogg opengl32 freetype winmm gdi32)
# set(CMAKE_CXX_FLAGS "-static -O2")