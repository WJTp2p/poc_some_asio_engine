# Создайте цель для вашей библиотеки ProtocolExample
add_library(ProtocolExample ProtocolExample.cpp)

# Подключите библиотеку nlohmann_json к вашей библиотеке
target_link_libraries(ProtocolExample)
