select cust_id,count(*) as orders
from orders
group by cust_id
having count(*) >= 2;


//列出具有2个及以上,价格为10及以上的供应商
select vend_id,count(*) as num_prods
from products
where prod_price >= 10
group by vend_id
having count(8) >= 2;

//group by 和order by结合使用
select vend_id,count(*) as num_prods
from products
where prod_price >= 10
group by vend_id
having count(8) >= 2
order by num_prods;

//子查询
select cust_name,cust_contact
from customers
where cust_id in(select cust_id
                 from orders
                 where order_num in(select order_num
                                   from orderitems
                                   where prod_id = 'TNT2'))
//连结实现上面的查询语句
select cust_name,cust_contact
from customers,orders,orderitems
where customers.cust_id = orders.cust_id
and   orderitems.order_num = orders.order_num
and   prod_id = 'TNT2';

//与上面功能相同只是使用了别名
select cust_name,cust_contact
from customers as c,orders as a,orderitems as oi;
where c.cust_id = o.cust_id and oi.order_num = o.order_num and prod_id = 'TNT2';


select cust_name
       cust_state
       (select count(*)
        from orders
        where orders.cust_id == customers.cust_id) as orders
from customers
order by cust_name;

//连结表
select vend_name prod_name,prod_price
from vendors,products
where vendors.vend_id = products.vend_id
order by vend_name prod_name;

//与上面的效果相同,内部连结
select vend_name,prod_name,prod_price
from vendors inner join products
on vendors.vend_id = products.vend_id;


//创建高级连结
//--->使用表别名

//--->使用不同类型的连结
//1.自联结,防止二义性
select p1.prod_id,p1.prod_name
from products as p1,products as p2
where p1.vend_id = p2.vend_id
and   p2.vend_id = 'DTNTR';


//外连结,左连接,左边都有，右边可能为空
select customers.cust_id,orders.order_num
from customers left outer join orders
on   customers.cust_id = orders.cust_id;

//使用带聚集函数的连结
select customers.cust_name,
       customers.cust_id,
       count(orders.order_num) as num_prod
from customers inner join orders
on customers.cust_id = orders.cust_id
group by customers.cust_id;

//组合查询
//表示两条select语句的联合
select vend_id.prod_id,prod_price
from products
where prod_price <= 5
union   //union all 不取消重复的行
select vend_id,prod_id,prod_price
from products
where vend_id in (1001,1002);

//对组合结果排序
select vend_id.prod_id,prod_price
from products
where prod_price <= 5
union   //union all 不取消重复的行
select vend_id,prod_id,prod_price
from products
where vend_id in (1001,1002)
order by vend_id,prod_price;

//全文本搜索


//插入数据
insert into customers(cust_id,
        cust_contact,
        cust_email,
        cust_name,
        cust_address,
        cust_city,
        cust_state,
        cust_zip,
        cust_country)
select cust_id, //或者用values包裹起来
       cust_contact,
       cust_email,
       cust_name,
       cust_address,
       cust_city,
       cust_state,
       cust_zip,
       cust_country
from custnew;

//更新和删除数据
update customers
set cust_name = 'The Fudds',
    cust_email = 'elmer@fudd.com'
where cust_id = 10005;

//删除行
delete from customers
where cust_id = 10006;

//创建和操纵表
create table customers
(
    cust_id         int         not null auto_increment,
    cust_name       char(50)    not null,
    cust_address    char(50)    null,
    cust_city       char(50)    null,
    cust_state      char(5)     null,
    cust_zip        char(10)    null,
    cust_country    char(50)    null,
    cust_contact    char(50)    null,
    cust_email      char(255)   null,
    primary key (cust_id)
)engin = innodb;

//修改表的结构
alter table vendors
add vend_phone char(20);

//删除添加的列
alter table vendors
drop column vend_phone;

//使用视图
//创建视图
create view productcustomers as
select cust_name,cust_contact,prod_id
from customers,orders,orderitems
where customers.cust_id = orders.cust_id
and   orderitems.order_num = orders.order_num;

//使用视图
select cust_name,cust_contact
from productcustomers
where prod_id = 'TNT2';

//更新视图

//使用存储过程
//为以后的使用而保存的一条或多条Mysql语句的集合,可将其视为批文件
//执行存储过程
call produntpricing(@pricelow,
                    @pricehigh,
                    @priceaverage);

//创建存储过程,无参数的存储过程
create procedure produntpricing()
begin 
    select Avg(prod_price) as priceaverage
    from products;
end;


//删除存储过程
drop procedure produntpricing;

//创建带参数的存储过程
create procedure productpricing(
    out p1 decimal(8,2),
    out ph decimal(8,2),
    out pa decimal(8,2),
)
begin
    select Min(prod_price)
    into p1
    from products;
    select Max(prod_price)
    int ph
    from productpricing;
    select Avg(prod_price)
    int pa
    from productpricing;
end;

//执行并不显示数据
call productpricing(@pricelow,
                    @pricehigh,
                    @priceaverage);
//使用返回的变量
select @priceaverage;

//使用游标
create procedure processorders()
begin
    declare ordernumbers cursor
    for
    select order_num from orderitems;

    open ordernumbers;
    close ordernumbers;

end;

//具体使用游标
create procedure processorders()
begin

//定义局部变量
declare done boolean default 0;
declare o int;
declare t decimal(8,2);

//定义游标
declare ordernumbers cursor
for
select order_num from orders;

//定义条件
declare continue handler for sqlstate '02000' set done = 1;

//创建一个表存储结果
create table if not exists ordertotals
(order_num int,total decimal(8,2));

//打开游标
open ordernumbers;

repeat
fetch ordernumbers into o;
call ordertotal(o,l,t);

insert into ordernumbers(order_num,total)
values(o,t);

until done end repeat;
close ordernumbers;

end;




//使用触发器
//在事件发生的时候自动执行
//创建触发器
create trigger newproduct after insert on products
for each row select 'Product added';

//一个表最多支持6个触发器,每条insert,update,delete的之前和之后



//管理事务处理
//rollback的使用
select * from ordertotals;
start transaction;
delete from ordertotals;
select * from ordernumbers;
rollback;
select * from ordernumbers;

//commit的使用







