bool Assert(bool condition, const std::string& message){
    if (!condition) {
        std::cerr << "Assertion failed: " << message << '\n';
        return false;
    }
    return true;
}
