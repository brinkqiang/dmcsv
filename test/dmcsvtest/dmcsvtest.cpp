#include "gtest.h"

#include "dmcsv.hpp"
#include "dmdsvfilter.hpp"

std::string strFile = "slist.csv";
std::queue<std::vector<std::string>> q;

TEST(dmcsv, dmcsv_init) {
    std::ofstream out(strFile);
    q.push({ "name", "age", "ip" });

    uint64_t qwCount = 0;

    for (int i = 0; i < 1000000; ++i)
    {
        qwCount += i;
        q.push({ "Andy" + std::to_string(i), std::to_string(i), "172.30.10.21" });
    }
    std::cout << qwCount << std::endl;
    out.close();
}

TEST(dmcsv, dmcsv_write) {
    std::ofstream out(strFile);
    auto writer = csv::make_csv_writer(out);

    for (; !q.empty(); q.pop())
        writer.write_row(q.front());

    out.close();
}

TEST(dmcsv, dmcsv_read_name) {

    uint64_t qwCount = 0;

    csv::CSVReader reader(strFile, csv::DEFAULT_CSV);
    csv::CSVRow rows;
    for (size_t i = 0; reader.read_row(rows); i++) {
        qwCount += rows["age"].get<uint64_t>();
    }
    std::cout << qwCount << std::endl;
}

TEST(dmcsv, dmcsv_read_index) {

    uint64_t qwCount = 0;

    csv::CSVReader reader(strFile, csv::DEFAULT_CSV);
    csv::CSVRow rows;
    for (size_t i = 0; reader.read_row(rows); i++) {
        qwCount += rows[1].get<uint64_t>();
    }
    std::cout << qwCount << std::endl;
}

TEST(dsv_filter, dsv_filter_read) {

    dsv_filter oFilter;
    if (!oFilter.load(strFile))
    {
        return;
    }

    uint64_t qwCount = 0;

    for (int i = 1; i < static_cast<int>(oFilter.grid().row_count()); ++i)
    {
        strtk::token_grid::row_type row = oFilter.grid().row(i);
        qwCount += row.get<uint64_t>(1);
    }

    std::cout << qwCount << std::endl;
}

TEST(dmformat, dmformat) {

}


TEST(TimerTest, BasicTiming) {
	strtk::util::timer t;
	t.start();

	// 模拟操作，耗时100ms
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	t.stop();
	double elapsed_time = t.time();

	// 检查时间大于或接近100ms (0.1秒)，放宽上下限范围
	ASSERT_GT(elapsed_time, 0.095);
	ASSERT_LT(elapsed_time, 0.115);

	ASSERT_FALSE(t.in_use());
}

TEST(TimerTest, InUseStatus) {
	strtk::util::timer t;

	// 初始化状态应为不使用
	ASSERT_FALSE(t.in_use());

	t.start();
	ASSERT_TRUE(t.in_use());

	t.stop();
	ASSERT_FALSE(t.in_use());
}

TEST(TimerTest, ConsecutiveMeasurements) {
	strtk::util::timer t;

	t.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	t.stop();
	double first_time = t.time();

	t.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	t.stop();
	double second_time = t.time();

	// 放宽范围：0.05秒到0.07秒
	ASSERT_GT(first_time, 0.045);
	ASSERT_LT(first_time, 0.070);

	// 放宽范围：0.095秒到0.115秒
	ASSERT_GT(second_time, 0.095);
	ASSERT_LT(second_time, 0.115);
}

TEST(TimerTest, ZeroTimeWhenNotStarted) {
	strtk::util::timer t;

	// 未启动计时器时调用time，不应该使用
	ASSERT_FALSE(t.in_use());
}

TEST(TimerTest, RepeatedStartWithoutStop) {
	strtk::util::timer t;
	t.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	t.start(); // 再次启动，覆盖之前的时间
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	t.stop();

	double elapsed_time = t.time();

	// 放宽范围：0.045秒到0.065秒
	ASSERT_GT(elapsed_time, 0.045);
	ASSERT_LT(elapsed_time, 0.065);
}