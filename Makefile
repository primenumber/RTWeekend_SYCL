TARGET   := rt_iow_gpu
SRCS     := src/main.cpp

CXX      := icpx
CXXFLAGS := -fsycl -Iinclude -std=c++17
LDFLAGS  :=

ifeq ($(BUILD_MODE), release)
    CXXFLAGS += -O3
else
    CXXFLAGS += -Og -g
endif

.PHONY: clean

$(TARGET): $(SRCS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	-$(RM) $(TARGET)

