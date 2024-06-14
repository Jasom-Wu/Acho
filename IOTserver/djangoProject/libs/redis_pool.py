import redis

# 抽取封装成模块，全局使用（单例模式，redis_pool.py）
POOL = redis.ConnectionPool(host='127.0.0.1', port=6379, password='', max_connections=1000,
                            db=0)  # db参数表示选择的数据库，redis总共有16个数据库，即0-15
