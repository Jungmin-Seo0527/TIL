# TIL

## 2. 스프링 핵심 원리 이해1 - 예제 만들기

### 2-1. 프로젝트 생성

#### build.gradle

```
plugins {
	id 'org.springframework.boot' version '2.5.1'
	id 'io.spring.dependency-management' version '1.0.11.RELEASE'
	id 'java'
}

group = 'hello'
version = '0.0.1-SNAPSHOT'
sourceCompatibility = '11'

repositories {
	mavenCentral()
}

dependencies {
	implementation 'org.springframework.boot:spring-boot-starter'
	testImplementation 'org.springframework.boot:spring-boot-starter-test'
}

test {
	useJUnitPlatform()
}

```

* 우선 순수 자바에서 부터 스프링까지 점진적으로 발전시키기 때문에 시작은 어떤것도 의존하지 않고 시작한다.
* Gradle 대신에 IntelliJ가 자바 직접 실행하도록 설정 변경

### 2-2. 비즈니스 요구사항과 설계

* 회원
    * 회원을 가입하고 조회할 수 있다.
    * 회원은 일반과 VIP 두 가지 등급이 있다.
    * 회원 데이터는 자체 DB를 구축할 수 있고, 외부 시스템과 연동할 수 있다.(미확정)

* 주문과 할인 정책
    * 회원은 상품을 주문할 수 있다.
    * 회원 등급에 따라 할인 정책을 적용할 수 있다.
    * 할인 정책은 모든 VIP는 1000원을 할인해주는 고정 금액 할인을 적용해달라.(나중에 변경 될 수 있다.)
    * 할인 정책은 변경 가능성이 높다. 회사의 기본 할인 정책을 아직 정하지 못했고, 오픈 직전까지 고민을 미루고 싶다. 최악의 경우 할인을 적용하지 않을 수 도 있다.(미확정)

요구사항을 보면 회원 데이터, 할인 정책 같은 부분은 지금 결정하기 어려운 부분이다. 그렇다고 이런 정책이 결정될 때 까지 개발을 무기한 기다릴 수 도 없다. 우리는 앞에서 배운 객체 지향 설계 방법이 있지
않은가!    
인터페이스를 만들고 구현체를 언제든지 갈아끼울 수 있도록 설계하면 된다.

### 2-3. 회원 도메인 설계

* 회원 도메인 요구사항
    * 회원을 가입하고 조회할 수 있다.
    * 회원은 일반과 VIP 두 가지 등급이 있다.
    * 회원 데이터는 자체 DB를 구출할 수 있고, 외부 시스템과 연동할 수 있다.(미확정)


* 회원 도메인 협력 관계
  ![](https://i.ibb.co/jJp5VqK/bandicam-2021-06-14-11-14-52-338.jpg)

* 회원 클래스 다이어그램
  ![](https://i.ibb.co/rMFPrYV/bandicam-2021-06-14-11-16-12-970.jpg)

* 회원 객체 다이어그램
  ![](https://i.ibb.co/8Bb4RfZ/bandicam-2021-06-14-11-16-55-936.jpg)

### 2-4. 회원 도메인 개발

#### 회원 엔티티

##### Grade.java - 회원 등급

* `src/main/java/hello/core1/member/Grade.java`

```java
package hello.core1.member;

public enum Grade {
    BASIC,
    VIP
}
```

##### Member.java - 회원 엔티티

* `src/main/java/hello/core1/member/Member.java`

```java
package hello.core1.member;

public class Member {

    private Long id;
    private String name;
    private Grade grade;

    public Member(Long id, String name, Grade grade) {
        this.id = id;
        this.name = name;
        this.grade = grade;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Grade getGrade() {
        return grade;
    }

    public void setGrade(Grade grade) {
        this.grade = grade;
    }
}

```

#### 회원 저장소

##### MemberRepository.java - 회원 저장소 인터페이스

* `src/main/java/hello/core1/member/MemberRepository.java`

```java
package hello.core1.member;

public interface MemberRepository {

    void save(Member member);

    Member findById(Long memberId);
}

```

##### MemoryMemberRepository.java - 메모리 회원 저장소 구현체

* `src/main/java/hello/core1/member/MemoryMemberRepository.java`

```java
package hello.core1.member;

import java.util.HashMap;
import java.util.Map;

public class MemoryMemberRepository implements MemberRepository {

    private static Map<Long, Member> store = new HashMap<>();

    @Override
    public void save(Member member) {
        store.put(member.getId(), member);
    }

    @Override
    public Member findById(Long memberId) {
        return store.get(memberId);
    }
}

```

데이터베이스가 아직 확정이 안되었다. 그래도 개발은 진행해야 하니 가장 단순한, 메모리 회원 저장소를 구현해서 우선 개발을 진행한다.

> 참고    
> `HashMap`은 동시성 이슈가 발생할 수 있다. 이런 경우 `ConcurrentHashMap`을 이용하자.

#### 회원 서비스

##### MemberService.java - 회원 서비스 인터페이스

* `src/main/java/hello/core1/member/MemberService.java`

```java
package hello.core1.member;

public interface MemberService {

    void join(Member member);

    Member findMember(Long memberId);
}

```

##### MemberServiceImpl.java - 회원 서비스 구현체

* `src/main/java/hello/core1/member/MemberServiceImpl.java`

```java
package hello.core1.member;

public class MemberServiceImpl implements MemberService {

    private final MemberRepository memberRepository = new MemoryMemberRepository();

    @Override
    public void join(Member member) {
        memberRepository.save(member);
    }

    @Override
    public Member findMember(Long memberId) {
        return memberRepository.findById(memberId);
    }
}

```

### 2-5. 회원 도메인 실행과 테스트

#### MemberApp.java - 회원 가입 main

* `src/main/java/hello/core1/MemberApp.java`

```java
package hello.core1;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;

public class MemberApp {

    public static void main(String[] args) {
        MemberService memberService = new MemberServiceImpl();
        Member member = new Member(1L, "memberA", Grade.VIP);
        memberService.join(member);

        Member findMember = memberService.findMember(1L);
        System.out.println("member = " + member.getName());
        System.out.println("findMember = " + findMember.getName());
    }
}

```

애플리케이션 로직으로 이렇게 테스트 하는 것은 좋은 방법이 아니다. JUnit 테스트를 사용하자.

#### MemberServiceTest.java - 회원 가입 테스트

* `src/test/java/hello/member/MemberServiceTest.java`

```java
package hello.core1.member;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;
import org.assertj.core.api.Assertions;
import org.junit.jupiter.api.Test;

public class MemberServiceTest {

    MemberService memberService = new MemberServiceImpl();

    @Test
    void join() {
        // given
        Member member = new Member(1L, "memberA", Grade.VIP);

        // when
        memberService.join(member);
        Member findMember = memberService.findMember(1L);

        // then
        Assertions.assertThat(member).isEqualTo(findMember);
    }
}

```

#### 회원 도메인 설계의 문제점

* 이 코드의 설계상 문제점은 무엇일까?
* 다른 저장소로 변경할 때 OCP 원칙을 잘 준수할까?
* DIP를 잘 지키고 있을까?
* **의존관계가 인터페이스 뿐만 아니라 구현까지 모두 의존하는 문제점이 있음**

### 2-6. 주문과 할인 도메인 설계

* 주문과 할인 정책
    * 회원은 상품을 주문할 수 있다.
    * 회원 등급에 따라 할인 정책을 적용할 수 있다.
    * 할인 정책은 모든 VIP는 1000원을 할인해주는 고정 금액 할인을 적용해달라. (나중에 변경 될 수 있다.)
    * 할인 정책은 변경 가능성이 높다. 회사의 기본 할인 정책을 아직 정하지 못했고, 오픈 직전까지 고민을 미루고 싶다. 최악의 경우 할인을 적용하지 않을 수 도 있다.(미확정)

#### 주문 도메인 협력, 역할, 책임

![](https://i.ibb.co/BKp65KX/bandicam-2021-06-14-12-26-59-570.jpg)

1. 주문생성: 클라이언트는 주문 서비스에 주문 생성을 요청한다.
2. 회원 조회: 할인을 위해서는 회원 등급이 필요하다. 그래서 주문 서비스는 회원 저장소에서 회원을 조회한다.
3. 할인 적용: 주문 서비스는 회원 등급에 따른 할인 여부를 할인 정책에 위임한다.
4. 주문 결과 반환: 주문 서비스는 할인 결과를 포함한 주문 결과를 반환한다.

#### 주문 도메인 전체

![](https://i.ibb.co/tpK27XB/bandicam-2021-06-14-12-28-44-507.jpg)

**역할과 구현을 분리**해서 자유롭게 구현 객체를 조립할 수 있게 설계했다. 덕분에 회원 저장소는 물론이고, 할인 정책도 유연하게 변경할 수 있다.

#### 주문 도메인 클래스 다이어그램

![](https://i.ibb.co/HhzWssX/bandicam-2021-06-14-12-29-53-626.jpg)

#### 주문 도메인 객체 다이어그램1

![](https://i.ibb.co/DKKNtYj/bandicam-2021-06-14-12-30-34-842.jpg)

회원을 메모리에서 조회하고, 정액 할인 정책(고정 금액)을 지원해도 주문 서비스를 변경하지 않아도 된다. 역할들의 협력 관계를 그대로 재사용 할 수 있다.

#### 주문 도메인 객체 다이어그램2

![](https://i.ibb.co/ccdYd52/bandicam-2021-06-14-12-31-29-475.jpg)

회원을 메모리가 아닌 실제 DB에서 조회하고, 정률 할인 정책(주문 금액에 따라 %할인)을 지원해도 주문 서비스를 변경하지 않아도 된다.    
협력 관계를 그대로 재사용 할 수 있다.

### 2-7. 주문과 할인 도메인 개발

#### DiscountPolicy.java - 할인 정책 인터페이스

* `src/main/java/hello/core1/discount/DiscountPolicy.java`

```java
package hello.core1.discount;

import hello.core1.member.Member;

public interface DiscountPolicy {

    /**
     * @return 할인 대상 금액
     */
    int discount(Member member, int price);
}
```

#### FixDiscountPolicy.java - 정액 할인 정책 구현체

* `src/main/java/hello/core1/discount/FixDiscountPolicy.java`

```java
package hello.core1.discount;

import hello.core1.member.Grade;
import hello.core1.member.Member;

public class FixDiscountPolicy implements DiscountPolicy {

    private int discountFixAmout = 1000;

    @Override
    public int discount(Member member, int price) {
        if (member.getGrade() == Grade.VIP) {
            return discountFixAmout;
        } else {
            return 0;
        }
    }
}
```

#### Order.java - 주문 엔티티

* `src/main/java/hello/core1/order/Order.java`

```java
package hello.core1.order;

public class Order {

    private Long memberId;
    private String itemName;
    private int itemPrice;
    private int discountPrice;

    public Order(Long memberId, String itemName, int itemPrice, int discountPrice) {
        this.memberId = memberId;
        this.itemName = itemName;
        this.itemPrice = itemPrice;
        this.discountPrice = discountPrice;
    }

    public int calculatePrice() {
        return itemPrice - discountPrice;
    }

    public Long getMemberId() {
        return memberId;
    }

    public void setMemberId(Long memberId) {
        this.memberId = memberId;
    }

    public String getItemName() {
        return itemName;
    }

    public void setItemName(String itemName) {
        this.itemName = itemName;
    }

    public int getItemPrice() {
        return itemPrice;
    }

    public void setItemPrice(int itemPrice) {
        this.itemPrice = itemPrice;
    }

    public int getDiscountPrice() {
        return discountPrice;
    }

    public void setDiscountPrice(int discountPrice) {
        this.discountPrice = discountPrice;
    }

    @Override
    public String toString() {
        return "Order{" +
                "memberId=" + memberId +
                ", itemName='" + itemName + '\'' +
                ", itemPrice=" + itemPrice +
                ", discountPrice=" + discountPrice +
                '}';
    }
}
```

#### OrderService.java - 주문 서비스 인터페이스

* `src/main/java/hello/core1/order/OrderService.java`

```java
package hello.core1.order;

public interface OrderService {

    Order createOrder(Long memberId, String itemName, int itemPrice);
}
```

#### OrderServiceImpl.java - 주문 서비스 구현체

* `src/main/java/hello/core1/order/OrderServiceImpl.java`

```java
package hello.core1.order;

import hello.core1.discount.DiscountPolicy;
import hello.core1.discount.FixDiscountPolicy;
import hello.core1.member.Member;
import hello.core1.member.MemberRepository;
import hello.core1.member.MemoryMemberRepository;

public class OrderServiceImpl implements OrderService {

    private final MemberRepository memberRepository = new MemoryMemberRepository();
    private final DiscountPolicy discountPolicy = new FixDiscountPolicy();

    @Override
    public Order createOrder(Long memberId, String itemName, int itemPrice) {
        Member member = memberRepository.findById(memberId);
        int discountPrice = discountPolicy.discount(member, itemPrice);

        return new Order(memberId, itemName, itemPrice, discountPrice);
    }
}
```

주문 생성 요청이 오면, 회원 정보를 조회하고, 할인 정책을 적용한 다음 주문 객체를 생성해서 반환한다.    
**메모리 회원 리포지토리와, 고정 금액 할인 정책을 구현체로 생성한다.**

### 2-8. 주문과 할인 도메인 실행과 테스트

#### OrderApp.java - 주문과 할인 정책 실행

* `src/main/java/hello/core1/OrderApp.java`
* JUnit을 사용하지 않는 테스트

```java
package hello.core1;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;
import hello.core1.order.Order;
import hello.core1.order.OrderService;
import hello.core1.order.OrderServiceImpl;

public class OrderApp {
    public static void main(String[] args) {
        MemberService memberService = new MemberServiceImpl();
        OrderService orderService = new OrderServiceImpl();

        Long memberId = 1L;
        Member member = new Member(memberId, "memberA", Grade.VIP);
        memberService.join(member);

        Order order = orderService.createOrder(memberId, "itemA", 10000);

        System.out.println("order = " + order);
        System.out.println("order.calculationPrice = " + order.calculatePrice());
    }
}
```

애플리케이션 로직으로 이렇게 테스트 하는 것은 좋은 방법이 아니다.

#### OrderServiceTest.java - 주문과 할인 정책 테스트(JUnit)

* `src/test/java/hello/core1/order/OrderServiceTest.java`

```java
package hello.core1.order;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;
import org.assertj.core.api.Assertions;
import org.junit.jupiter.api.Test;

class OrderServiceTest {

    MemberService memberService = new MemberServiceImpl();
    OrderService orderService = new OrderServiceImpl();

    @Test
    void createOrder() {
        // given
        Long memberId = 1L;
        Member member = new Member(memberId, "memberA", Grade.VIP);
        memberService.join(member);

        // when
        Order order = orderService.createOrder(memberId, "itemA", 10000);

        // then
        Assertions.assertThat(order.getDiscountPrice()).isEqualTo(1000);
    }
}
```

## 3. 스프링 핵심 원리 이해2 - 객체 지향 원리 적용

### 3-1. 새로운 할인 정책 개발

주문한 금액의 %를 할인해 주는 새로운 정률 할인 정책 추가

#### RateDiscountPolicy.java - 정률 할인 정책

* `src/main/java/hello/core1/discount/RateDiscountPolicy.java`

```java
package hello.core1.discount;

import hello.core1.member.Grade;
import hello.core1.member.Member;

public class RateDiscountPolicy implements DiscountPolicy {

    private final int discountPercent = 10;

    @Override
    public int discount(Member member, int price) {
        if (member.getGrade() == Grade.VIP) {
            return price * discountPercent / 100;
        } else {
            return 0;
        }
    }
}
```

#### RateDiscountPolicyTest.java - 테스트 코드

* `src/test/java/hello/core1/discount/RateDiscountPolicyTest.java`

```java
package hello.core1.discount;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.assertj.core.api.Assertions.*;

class RateDiscountPolicyTest {

    RateDiscountPolicy discountPolicy = new RateDiscountPolicy();

    @Test
    @DisplayName("VIP는 10% 할인이 적용되어야 한다.")
    void vip_o() {
        // given
        Member member = new Member(1L, "memberVIP", Grade.VIP);

        // when
        int discount = discountPolicy.discount(member, 10000);

        // then
        assertThat(discount).isEqualTo(1000);
    }

    @Test
    @DisplayName("VIP가 아니면 할인이 적용되지 않아야 한다.")
    void vip_x() {
        // given
        Member member = new Member(2L, "memberBASIC", Grade.BASIC);

        // when
        int discount = discountPolicy.discount(member, 10000);

        // then
        assertThat(discount).isEqualTo(0);
    }
}
```

### 3-2. 새로운 할인 정책 적용과 문제점

#### 할인정책을 애플리케이션에 적용하기

```java
package hello.core1.order;

import hello.core1.discount.DiscountPolicy;
import hello.core1.member.Member;
import hello.core1.member.MemberRepository;
import hello.core1.member.MemoryMemberRepository;

public class OrderServiceImpl implements OrderService {

    private final MemberRepository memberRepository = new MemoryMemberRepository();
    private final DiscountPolicy discountPolicy = new RateDiscountPolicy();

    public Order createOrder(Long memberId, String itemName, int itemPrice) {
        Member member = memberRepository.findById(memberId);
        int discountPrice = discountPolicy.discount(member, itemPrice);

        return new Order(memberId, itemName, itemPrice, discountPrice);
    }
}

```

* 문제점 발견
    * 우리는 역할과 구현을 충실하게 분리했다. -> OK
    * 다형성도 활용하고, 인터페이스와 구현 객체를 분리했다. -> OK
    * OCP, DIP 같은 객체지향 설계 원칙을 충식히 준수했다.
        * 그렇게 보이지만 사실은 아니다.
    * DIP: 주문 서비스 클라이언트(`OrderServiceImpl`)는 `DiscoutPolicy`인터페이스에 의존하면서 DIP를 지킨것 같지만...
        * 클래스 의존관계를 분석해보자. 추상(인터페이스) 뿐만 아니라 **구체(구현) 클래스에도 의존**하고 있다.
            * 추상(인터페이스)의존: `DiscountPolicy`
            * 구체(구현)클래스: `FixDiscountPolicy`, `RateDiscountPolicy`
    * OCP: 변경하지 않고 확장할 수 있다고 했는데
        * 지금 코드는 기능을 확장해서 변경하면, 클라이언트 코드에 영향을 준다! 따라서 OCP를 위반한다.

#### 할인정책 관련 클래스 다이어그램 분석

* 기대했던 의존관계
  ![](https://i.ibb.co/FqD3wSs/bandicam-2021-06-14-18-55-00-459.jpg)
    * 지금까지 단순히 `DiscountPolicy`인터페이스만 의존한다고 생각했다.

* 실제 의존 관계
  ![](https://i.ibb.co/gD5h1kc/bandicam-2021-06-14-18-55-57-900.jpg)
    * 잘보면 클라이언트인 `OrderServiceImpl`이 `DiscountPolicy`뿐만 아니라 `FixDiscountPolciy`인 구체 클래스도 함께 의존하고 있다. 실제 코드를 보면 의존하고
      있다! **DIP 위반**

* 정책 변경
  ![](https://i.ibb.co/WWFzd9q/bandicam-2021-06-14-18-57-50-219.jpg)
    * **중요!!!**: 그래서 `FixDiscountPolicy`를 `RateDiscountPolicy`로 변경하는 순간 `OrderServiceImpl`의 소스 코드도 함께 변경해야 한다. **OCP
      위반**

#### 어떻게 문제를 해결할 수 있을까???

* 클라이언트 코드인 `OrderServiceImpl`은 `DiscountPolicy`의 인터페이스 뿐만 아니라 구체 클래스도 함께 의존한다.
* 그래서 구체 클래스를 변경할 때 클라이언트 코드도 함께 변경해야 한다.
* **DIP 위반** -> 추상에만 의존하도록 변경(인터페이스에만 의존)
* DIP를 위반하지 않도록 인터페이스에만 의존하도록 의존관계를 변경하면 된다.
  ![](https://i.ibb.co/VWY7JMh/bandicam-2021-06-14-19-01-04-202.jpg)

##### OrderServiceImpl.java(수정) - 인터페이스에만 의존하도록 코드 변경

```java
public class OrderServiceImpl implements OrderService {
    // private final DiscountPolicy discountPolicy = new RateDiscountPolicy();
    private final DiscountPolicy discountPolicy;
}
```

* 인터페이스에만 의존하도록 설계와 코드를 변경했다.
* **그런데 구현체가 없는데 어떻게 코드를 실행할 수 있을까?**
* 실제 실행을 해보면 NPE(Null Pointer Exception)가 발생한다.


* **해결방안**
* 이 문제를 해결하려면 누군가가 클라이언트인 `OrderServiceImapl`에 `DiscountPolicy`의 구현 객체를 대신 생성하고 주입해주어야 한다.

### 3-3. 관심사의 분리

* 애플리케이션을 하나의 공연이라고 생각해보자. 각각의 인터페이스를 배역(배우 역할)이라 생각하자. 그런데 실제 배역에 맞는 배우를 선택하는 것은 누가 하는가?
* 로미오와 줄리엣 공연을 하면 로미오 역할을 누가 할지 줄리엣 역할을 누가 할지는 배우들이 정하는게 아니다. 이전 코드는 마치 로미오 역할(인터페이스)을 하는 레오나르도 디카프리오(구현체, 배우)가 줄리엣 역할(
  인터페이스)을 하는 여자 주인공(구현체, 배우)을 직접 초빙하는 것과 같다. 디카프리오는 공연도 해야하고 동시에 여자 주인공도 공연에 직접 초빙해야 하는 **다양한 책임**을 가지고 있다.

#### 관심사 분리하기

* 배우는 본인의 역할인 배역을 수행하는 것에만 집중해야 한다.
* 디카프리오는 어떤 여자 주인공이 선택되더라도 똑같이 공연을 할 수 있어야 한다. 공연을 구성하고, 담당 배우를 섭외하고, 역할에 맞는 배우를 지정하는 책임을 담당하는 별도의 **공연 기획자**가 나올 시점이다.
    * 공연 기획자를 만들고, 배우와 공연 기획자의 책임을 확실히 분리하자.

#### AppConfig.java - 설정 클래스

* 애플리케이션의 전체 동작 방식을 구성(config)하기 위해, **구현 객체를 생성**하고, **연결**하는 책임을 가지는 별도의 설정 클래스를 만든다.

* `src/main/java/hello/core1/AppConfig.java`

```java
package hello.core1;

import hello.core1.discount.FixDiscountPolicy;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;
import hello.core1.member.MemoryMemberRepository;
import hello.core1.order.OrderService;
import hello.core1.order.OrderServiceImpl;

public class AppConfig {

    public MemberService memberService() {
        return new MemberServiceImpl(new MemoryMemberRepository());
    }

    public OrderService orderService() {
        return new OrderServiceImpl(new MemoryMemberRepository(), new FixDiscountPolicy());
    }
}
```

* `AppConfig`는 애플리케이션의 실제 동작에 필요한 **구현 객체를 생성**한다.
    * `MemberServiceImpl`
    * `MemoryMemberRepository`
    * `OrderServiceImpl`
    * `FixDiscountPolicy`

* `AppConfig`는 생성한 객체 인스턴스의 참조(래퍼런스)를 **생성자를 통해서 주입(연결)** 해준다.
    * `MemberServiceImpl` -> `MemoryMemberRepository`
    * `OrderServiceImpl` -> `MemoryMemberRepository`, `FixDiscountPolicy`

> 참고    
> 지금은 각 클래스에 생성자가 없어서 컴파일 오류가 발생한다. 바로 다음에 코드에서 생성자를 만든다.

#### MemberServiceImpl.java (수정) - 생성자 주입

```java
package hello.core1.member;

public class MemberServiceImpl implements MemberService {

    private final MemberRepository memberRepository;

    public MemberServiceImpl(MemberRepository memberRepository) {
        this.memberRepository = memberRepository;
    }

    @Override
    public void join(Member member) {
        memberRepository.save(member);
    }

    @Override
    public Member findMember(Long memberId) {
        return memberRepository.findById(memberId);
    }
}

```

* 실제 변경으로 `MemberServiceImpl`은 `MemoryMemberRepository`를 의존하지 않는다.
* 단지 `MemberRepository`인터페이스만 의존한다.
* `MemberServiceImpl`입장에서 생성자를 통해 어떤 구현 객체가 들어올지(주입될지)는 알 수 없다.
* `MemberServiceImpl`의 생성자를 통해서 어떤 구현 객체를 주입할지는 오직 외부(`AppConfig`)에서 결정된다.
* `MemberServiceImpl`은 이제부터 **의존관계에 대한 고민은 외부**에 맡기고 **실행에만 집중**하면 된다.

#### AppConfig를 추가한 클래스 다이어그램

![](https://i.ibb.co/NVRzGb6/bandicam-2021-06-14-21-33-40-705.jpg)

* 객체의 생성과 연결은 `AppConfig`가 담당한다.
* **DIP완성**: `MemberServiceImpl`은 `MemberRepository`인 추상에만 의존하면 된다. 이제 구체 클래스를 몰라도 된다.
* **관심사의 분리**: 객체를 생성하고 연결하는 역할과 실행하는 역할이 명확히 분리되었다.

#### 회원 객체 인스턴스 다이어그램

![](https://i.ibb.co/gwQt074/bandicam-2021-06-14-21-35-54-024.jpg)

* `appConfig`객체는 `memoryMemberRepository`객체를 생성하고 그 참조값을 `memberServiceImpl`을 생성하면서 생성자로 전달한다.
* 클라이언트인 `memeberServiceImpl`입장에서 보면 의존관계를 마치 외부에서 주입해주는 것 같다고 해서 DI(Dependency Injection)우리말로 의존관계 주입 또는 의존성 주입이라 한다.

#### OrderServiceImpl.java(수정) - 생성자 주입

```java
package hello.core1.order;

import hello.core1.discount.DiscountPolicy;
import hello.core1.member.Member;
import hello.core1.member.MemberRepository;
import hello.core1.member.MemoryMemberRepository;

public class OrderServiceImpl implements OrderService {

    private final MemberRepository memberRepository;
    private final DiscountPolicy discountPolicy;

    public OrderServiceImpl(MemberRepository memberRepository, DiscountPolicy discountPolicy) {
        this.memberRepository = memberRepository;
        this.discountPolicy = discountPolicy;
    }

    @Override
    public Order createOrder(Long memberId, String itemName, int itemPrice) {
        Member member = memberRepository.findById(memberId);
        int discountPrice = discountPolicy.discount(member, itemPrice);

        return new Order(memberId, itemName, itemPrice, discountPrice);
    }
}
```

* 설정 변경으로 `OrderServiceImpl`은 `FixDiscountPolicy`를 의존하지 않는다.
* 단지 `DiscountPolicy`인터페이스만 의존한다.
* `OrderServiceImpl`입장에서 생성자를 통해 어떤 구현 객체가 들어올지(주입될지)는 알 수 없다.
* `OrderServiceImpl`의 생성자를 통해서 어떤 구현 객체를 주입할지는 오직 외부(`AppConfig`)에서 결정한다.
* `OrderServiceImpl`는 이제부터 실행에만 집중하면 된다.
* `OrderServiceImpl`에는 `MemoryMemberRepository`, `FixDiscountPolicy`객체의 의존관계가 주입된다.

#### AppConfig 실행

##### MemberApp.java (수정) - AppConfig 사용

```java
package hello.core1;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;

public class MemberApp {

    public static void main(String[] args) {
        AppConfig appConfig = new AppConfig();
        MemberService memberService = appConfig.memberService();
        Member member = new Member(1L, "memberA", Grade.VIP);
        memberService.join(member);

        Member findMember = memberService.findMember(1L);
        System.out.println("member = " + member.getName());
        System.out.println("findMember = " + findMember.getName());
    }
}

```

##### OrderApp.java(수정) - AppConfig 사용

```java
package hello.core1;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;
import hello.core1.order.Order;
import hello.core1.order.OrderService;

public class OrderApp {
    public static void main(String[] args) {
        AppConfig appConfig = new AppConfig();
        MemberService memberService = appConfig.memberService();
        OrderService orderService = appConfig.orderService();

        Long memberId = 1L;
        Member member = new Member(memberId, "memberA", Grade.VIP);
        memberService.join(member);

        Order order = orderService.createOrder(memberId, "itemA", 10000);

        System.out.println("order = " + order);
        System.out.println("order.calculationPrice = " + order.calculatePrice());
    }
}

```

##### MemberServiceTest.java(수정) - AppConfig 사용

```java
package hello.core1.member;

import hello.core1.AppConfig;
import org.assertj.core.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class MemberServiceTest {

    MemberService memberService;

    @BeforeEach
    public void beforeEach() {
        AppConfig appConfig = new AppConfig();
        memberService = appConfig.memberService();
    }

    @Test
    void join() {
        // given
        Member member = new Member(1L, "memberA", Grade.VIP);

        // when
        memberService.join(member);
        Member findMember = memberService.findMember(1L);

        // then
        Assertions.assertThat(member).isEqualTo(findMember);
    }
}

```

##### OrderServiceTest.java(수정) - AppConfig 사용

```java
package hello.core1.order;

import hello.core1.AppConfig;
import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;
import org.assertj.core.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class OrderServiceTest {

    MemberService memberService;
    OrderService orderService;

    @BeforeEach
    void beforeEach() {
        AppConfig appConfig = new AppConfig();
        memberService = appConfig.memberService();
        orderService = appConfig.orderService();
    }

    @Test
    void createOrder() {
        // given
        Long memberId = 1L;
        Member member = new Member(memberId, "memberA", Grade.VIP);
        memberService.join(member);

        // when
        Order order = orderService.createOrder(memberId, "itemA", 10000);

        // then
        Assertions.assertThat(order.getDiscountPrice()).isEqualTo(1000);
    }
}
```

* 테스트 코드에서 `@BeforeEach`는 각 테스트를 실행하기 전에 호출된다.

#### 정리

* AppConfig를 통해서 관심사를 확실하게 분리했다.
* 배역, 배우를 생각해보자.
* AppConfig는 공연 기획자다.
* AppConfig는 구체 클래스를 선택한다. 배역에 맞는 담당 배우를 선택한다. 애플리케이션이 어떻게 동작해야 할지 전체 구성을 책임진다.
* 이제 각 배우들은 담당 기능을 실행하는 책임만 지면 된다.
* `OrderServiceImpl`은 기능을 실행하는 책임만 지면 된다.

### 3-4. AppConfig 리팩터링

현재 `AppConfig`를 보면 중복이 있고, 역할에 따른 구현이 잘 안보인다.

![](https://i.ibb.co/PFqdWTd/bandicam-2021-06-15-10-41-07-174.jpg)

#### AppConfig.java(수정) - 리팩터링

```java
package hello.core1;

import hello.core1.discount.DiscountPolicy;
import hello.core1.discount.FixDiscountPolicy;
import hello.core1.member.MemberRepository;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;
import hello.core1.member.MemoryMemberRepository;
import hello.core1.order.OrderService;
import hello.core1.order.OrderServiceImpl;

public class AppConfig {

    public MemberService memberService() {
        return new MemberServiceImpl(memberRepository());
    }

    private MemberRepository memberRepository() {
        return new MemoryMemberRepository();
    }

    public OrderService orderService() {
        return new OrderServiceImpl(memberRepository(), discountPolicy());
    }

    private DiscountPolicy discountPolicy() {
        return new FixDiscountPolicy();
    }
}

```

* `new MemoryMemberRepository()` 이 부분이 중복 제거 되었다. 이제 `MemoryMemberRepository`를 다른 구현체로 변경할 때 한 부분만 변경하면 된다.
* `AppConfig`를 보면 역할과 구현 클래스가 한눈에 들어온다. 애플리케이션 전체 구성이 어떻게 되어있는지 빠르게 파악할 수 있다.

### 3-5. 새로운 구조와 할인 정책 적용

* 처음으로 돌아가서 정액 할인 정책을 정률% 할인 정책으로 변경해보자.
* `FixDiscountPolicy` -> `RateDiscountPolicy`
* 어떤 부분만 변경하면 될까?

**`AppConfig`의 등장으로 애플리케이션이 크게 사용 영역과, 객체를 생성하고 구성(Configuration)하는 영역으로 분리되었다.**

* 사용, 구성의 분리
  ![](https://i.ibb.co/FXC70yB/bandicam-2021-06-15-10-53-05-172.jpg)

* 할인 정책 변경
  ![](https://i.ibb.co/N2Ysbc5/bandicam-2021-06-15-10-53-36-718.jpg)

* `FixDiscountPolicy` -> `RateDiscountPolicy`로 변경해도 구성 영역만 영향을 받고, 사용 영역은 전혀 영향을 받지 않는다.

#### AppConfig.java(수정) - 할인 정책 변경

```java
package hello.core1;

import hello.core1.discount.DiscountPolicy;
import hello.core1.discount.FixDiscountPolicy;
import hello.core1.discount.RateDiscountPolicy;
import hello.core1.member.MemberRepository;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;
import hello.core1.member.MemoryMemberRepository;
import hello.core1.order.OrderService;
import hello.core1.order.OrderServiceImpl;

public class AppConfig {

    public MemberService memberService() {
        return new MemberServiceImpl(memberRepository());
    }

    private MemberRepository memberRepository() {
        return new MemoryMemberRepository();
    }

    public OrderService orderService() {
        return new OrderServiceImpl(memberRepository(), discountPolicy());
    }

    private DiscountPolicy discountPolicy() {
        return new RateDiscountPolicy();
    }
}

```

* `AppConfig`에서 할인 정책 역할을 담당하는 구현을 `FixDiscountPolicy` -> `RateDiscountPolicy`객체로 변경했다.
* 이제 할인 정책을 변경해도, 애플리케이션의 구성 역할을 담당했던 AppConfig만 변경하면 된다. 클라이언트 코드인 `OrderServiceImpl`를 포함해서 **사용 영역**의 어떤 코드도 변경할 필요가
  없다.
* **구성 영역**은 당연히 변경된다. 구성 역할을 담당하는 `AppConfig`를 애플리케이션이라는 공연의 기획자로 생각자하자. 공연 기획자는 공연 참여자인 구현 객체들을 모두 알아야 한다.

### 3-6. 전체 흐름 정리

* 새로운 할인 정책 개발
* 새로운 할인 정책 적용과 문제점
* 관심사의 분리
* AppConfig 리팩터링
* 새로운 구조와 할인 정책 적용

#### 새로운 할인 정책 개발

다형성 덕분에 새로운 정률 할인 정책 코드를 추가로 개발하는 것 자체는 아무 문제가 없음

#### 새로운 할인 정책 적용과 문제점

새로 개발한 정률 할인 정책을 적용하려고 하니 **클라이언트 코드**인 주문 서비스 구현체도 함께 변경해야 함   
주문 서비스 클라이언트가 인터페이스인 `DiscountPolicy`뿐만 아니라, 구체 클래스인 `FixDiscountPolicy`도 함께 의존 -> **DIP 위반**

#### 관심사 분리

* 애플리케이션을 하나의 공연으로 생각
* 기존에는 클라이언트가 의존하는 서버 구현 객체를 직접 생성하고, 실행함
* 비유를 하면 기존에는 남자 주인공 배우가 공연도 하고, 동시에 여자 주인공도 직접 초빙하는 다양한 책음을 가지고 있음
* 공연을 구성하고, 담당 배우를 섭외하고, 지정하는 책임을 담당하는 별도의 **공연 기획자**가 나올 시점
* 공연 기획자인 `AppConfig`가 등장
* `AppConfig`는 애플리케이션의 전체 동작 방식을 구성(config)하기 위해, **구현 객체를 생성**하고, **연결**하는 책임
* 이제부터 클라이언트 객체는 자신의 역할을 실행하는 것만 집중, 권한이 줄어듬(책임이 명확해짐)

#### AppConfig 리펙터링

* 구성 정보에서 역할과 구현을 명확하게 분리
* 역할이 잘 드러남
* 중복 제거

#### 새로운 구조와 할인 정책 적용

* 정액 할인 정책 -> 정률% 할인 정책으로 변경
* `AppConfig`의 등장으로 애플리케이션이 크게 **사용 영역**과, 객체를 생성하고 **구성(Configuration)하는 영역**으로 분리
* 할인 정책을 변경해도 `AppConfig`가 있는 구성 영역만 변경하면 됨, 사용 영역은 변경할 필요가 없음. 물론 클라이언트 코드인 주문 서비스 코드도 변경하지 않음

### 3-7. 좋은 객체 지향 설계의 5가지 원칙의 적용

여기서 3가지 SRP, DIP, OCP 적용

#### SRP 단일 책임 원칙

**한 클래스는 하나의 책임만 가져야 한다.**

* 클라이언트 객체는 직접 구현 객체를 생성하고, 연결하고, 실행하는 다양한 책임을 가지고 있음
* SRP 단일 책임 원책을 따르면서 관심사를 분리함
* 구현 객체를 생성하고 연결하는 책임은 `AppConfig`가 담당
* 클라이언트 객체는 실행하는 책임만 담당

#### DIP 의존관계 역전 원칙

**프로그래머는 "추상화에 의존해야지, 구체화에 의존하면 안된다." 의존성 주입은 이 원칙을 따르는 방법 중 하나이다.**

* 새로운 할인 정책을 개발하고, 적용하려고 하니 클라이언트 코드도 함께 변경해야 했다. 왜냐하면 기존 클라이언트 코드(`OrderServiceImpl`)는 DIP를 지키며 `DiscountPolicy`추상화
  인터페이스에 의존하는 것 같았지만, `FixDiscountPolicy`구체화 구현 클래스에도 함께 의존했다.
* 클라이언트 코드가 `DiscountPolicy` 추상화 인터페이스에만 의존하도록 코드를 변경했다.
* 하지만 클라이언트 코드는 인터페이스만으로는 아무것도 실행할 수 없다.
* `AppConfig`가 `FixDiscountPolicy`객체 인스턴스를 클라이언트 코드 대신 생성해서 클라이언트 코드에 의존관계를 주입했다. 이렇게 해서 DIP 원칙을 따르면서 문제도 해결했다.

#### OCP

**소프트웨어 요소는 확장에는 열려 있으나 변경에는 닫혀 있어야 한다.**

* 다형성을 사용하고 클라이언트가 DIP를 지킴
* 애플리케이션을 사용 영역과 구성 영역으로 나눔
* `AppConfig`가 의존관계를 `FixDiscountPolicy` -> `RateDiscountPolicy`로 변경해서 클라이언트 코드에 주입하므로 클라이언트 코드는 변경하지 않아도 됨
* **소프트웨어 요소를 새롭게 확장해도 사용 영역의 변경은 닫혀 있다!**

### 3-8. IoC, DI, 그리고 컨테이너

#### 제어의 역전 IoC(Inversion of Control)

* 기존 프로그램은 클라이언트 구현 객체가 스스로 필요한 서버 구현 객체를 생성 하고, 연결 하고, 실행했다. 한 마디로 구현 객체가 프로그램의 제어 프름을 스스로 조종했다. 개발자 입장에서는 자연스러운 흐름이다.
* 반면에 `AppConfig`가 등장한 이후에 구현 객체는 자신의 로직을 실행하는 역할만 담당한다. 프로그램의 제어 프름은 이제 `AppConfig`가 가져간다. 예를 들어서 `OrderServiceImpl`은
  필요한 인터페이스들을 호출하지만 어떤 구현 객체들이 실행될지 모른다.
* 프로그램에 대한 제어 프름에 대한 권한은 모두 `AppConfig`가 가지고 있다. 심지어 `OrderServiceImpl`도 `AppConfig`가 생성한다. 그리고 `AppConfig`
  는 `OrderServiceImpl`이 아닌 `OrderService`인터페이스의 다른 구현 객체를 생성하고 실행할 수 도 있다. 그런 사실도 모른체 `OrderServiceImpl`은 묵묵히 자신의 로직을
  실행할 뿐이다.
* 이렇듯 프로그램의 제어 흐름을 직접 제어하는 것이 아니라 외부에서 관리하는 것을 제어의 역전(IoC)이라 한다.

#### 프레임워크 vs 라이브러리

* 프레임워크가 내가 작성한 코드를 제어하고, 대신 실행하면 그것은 프레임워크가 맞다.(JUnit)
* 반면에 내가 작성한 코드가 직접 제어의 흐름을 담당한다면 그것은 프레임워크가 아니라 라이브러리다.

#### 의존관계 주입 DI(Dependency Injection)

* `OrderServiceImpl`은 `DiscountPolicy`인터페이스에 의존한다. 실제 어떤 구현 객체가 사용될지는 모른다.
* 의존관계는 **정적인 클래스 의존 관계와, 실행 시점에 결정되는 동적인 객체(인스턴스) 의존 관계 둘을 분리해서 생각해야 한다.**

##### 정적인 클래스 의존관계

클래스가 사용하는 import 코드만 보고 의존관계를 쉽게 판단할 수 있다. 정적인 의존관계는 애플리케이션을 실행하지 않아도 분석할 수 있다. 클래스 다이어그램을 보자.    
`OrderServiceImpl`은 `MemberRepository`, `DiscountPolicy`에 의존한다는 것을 알 수 있다.    
그런데 이러한 클래스 의존관계 만으로는 실제 어떤 객체가 `OrderServiceImpl`에 주입 될지 알 수 없다.

##### 클래스 다이어그램

![](https://i.ibb.co/tMRPHGt/bandicam-2021-06-15-12-11-30-710.jpg)

##### 객체 다이어그램

![](https://i.ibb.co/RCs0g82/bandicam-2021-06-15-12-12-06-669.jpg)

* 동적인 객체 인스턴스 의존관계
    * 애플리케이션 실행 시점에 실제 생성된 객체 인스턴스의 참조가 연결된 의존 관계다.

* 애플리케이션 **실행 시점(런타임)** 에 외부에서 실제 구현 객체를 생성하고 클라이언트에 전달해서 클라이언트와 서버의 실제 의존관계가 연결 되는 것을 **의존관계 주입**이라 한다.
* 객체 인스턴스를 생성하고, 그 참조값을 전달해서 연결된다.
* 의존관계 주입을 사용하면 클라이언트 코드를 변경하지 않고, 클라이언트가 호출하는 대상의 타입 인스턴스를 변경할 수 있다.
* 의존관계 주입을 사용하면 정적인 클래스 의존관계를 변경하지 않고, 동적인 객체 인스턴스 의존관계를 쉽게 변경할 수 있다.

#### IoC 컨테이너, DI 컨테이너

* `AppConfig`처럼 객체를 생성하고 관리하면서 의존관계르 연결해 주는 것을 IoC 컨테이너 또는 **DI 컨테이너**fk gksek.
* 의존관계 주입에 초점을 맞추어 최근에는 주로 DI 컨테이너라 한다.
* 또는 어셈블러, 오브젝트 팩토리 등으로 불리기도 한다.

### 3-9. 스프링으로 전환하기

#### AppConfig.java(수정) - 스프링으로 전환

```java
package hello.core1;

import hello.core1.discount.DiscountPolicy;
import hello.core1.discount.FixDiscountPolicy;
import hello.core1.discount.RateDiscountPolicy;
import hello.core1.member.MemberRepository;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;
import hello.core1.member.MemoryMemberRepository;
import hello.core1.order.OrderService;
import hello.core1.order.OrderServiceImpl;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class AppConfig {

    @Bean
    public MemberService memberService() {
        return new MemberServiceImpl(memberRepository());
    }

    @Bean
    public MemberRepository memberRepository() {
        return new MemoryMemberRepository();
    }

    @Bean
    public OrderService orderService() {
        return new OrderServiceImpl(memberRepository(), discountPolicy());
    }

    @Bean
    public DiscountPolicy discountPolicy() {
        return new RateDiscountPolicy();
    }
}

```

* `AppConfig`에 설정을 구성한다는 뜻의 `@Configuration`을 붙여준다.
* 각 메서드에 `@Bean`을 붙여준다. 이렇게 하면 스프링 컨테이너에 스프링 빈으로 등록한다.

#### MemberApp.java(수정) - 스프링 컨테이너 적용

```java
package hello.core1;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class MemberApp {

    public static void main(String[] args) {
        // AppConfig appConfig = new AppConfig();
        // MemberService memberService = appConfig.memberService();

        ApplicationContext applicationContext = new AnnotationConfigApplicationContext(AppConfig.class);
        MemberService memberService = applicationContext.getBean("memberService", MemberService.class);

        Member member = new Member(1L, "memberA", Grade.VIP);
        memberService.join(member);

        Member findMember = memberService.findMember(1L);
        System.out.println("member = " + member.getName());
        System.out.println("findMember = " + findMember.getName());
    }
}

```

#### OrderApp.java(수정) - 스프링 컨테이너 적용

```java
package hello.core1;

import hello.core1.member.Grade;
import hello.core1.member.Member;
import hello.core1.member.MemberService;
import hello.core1.order.Order;
import hello.core1.order.OrderService;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class OrderApp {
    public static void main(String[] args) {
        // AppConfig appConfig = new AppConfig();
        // MemberService memberService = appConfig.memberService();
        // OrderService orderService = appConfig.orderService();

        ApplicationContext applicationContext = new AnnotationConfigApplicationContext(AppConfig.class);
        MemberService memberService = applicationContext.getBean("memberService", MemberService.class);
        OrderService orderService = applicationContext.getBean("orderService", OrderService.class);

        Long memberId = 1L;
        Member member = new Member(memberId, "memberA", Grade.VIP);
        memberService.join(member);

        Order order = orderService.createOrder(memberId, "itemA", 10000);

        System.out.println("order = " + order);
        System.out.println("order.calculationPrice = " + order.calculatePrice());
    }
}

```

#### 스프링 컨테이너

* `ApplicationContext`를 스프링 컨테이너라 한다.
* 기존에는 개발자가 `AppConfig`를 사용해서 직접 객체를 생성하고 DI를 했지만, 이제부터는 스프링 컨테이너를 통해서 사용한다.
* 스프링 컨테이너는 `@Configuration`이 붙은 `AppConfig`를 설정(구성)정보로 사용한다. 여기서 `@Bean`이라 적힌 메서드를 모두 호출해서 반환된 객체를 스프링 컨테이너에 등록한다. 이렇게
  스프링 컨테이너에 등록된 객체를 스프링 빈이라 한다.
* 스프링 빈은 `@Bean`이 붙은 메서드의 명을 스프링 빈의 이름으로 사용한다. (`memberService`, `orderService`)
* 이전에는 개발자가 필요한 객체를 `AppConfig`를 사용해서 직접 조회했지만, 이제부터는 스프링 컨테이너를 통해서 필요한 스프링 빈(객체)를 찾아야 한다. 스프링
  빈은 `applicationContext.getBean()`메서드를 사용해서 찾을 수 있다.
* 기존에는 개발자가 직접 자바코드로 모든 것을 했다면 이제부터는 스프링 컨테이너에 객체를 스프링 빈으로 등록하고, 스프링 컨테이너에서 스프링 빈을 찾아서 사용하도록 변경되었다.

## 4. 스프링 컨테이너와 스프링 빈

### 4-1. 스프링 컨테이너 생성

1. **스프링 컨테이너 생성**
   ![](https://i.ibb.co/bRLhcG1/bandicam-2021-06-15-13-49-31-404.jpg)
    * `new AnnotationConfigApplicationContext(AppConfig.class)`
    * 스프링 컨테이너를 생성할 때는 구성 정보를 지정해주어야 한다.
    * 여기서는`AppConfig.class`를 구성 정보로 지정했다.


2. **스프링 빈 등록**
   ![](https://i.ibb.co/VD63jN1/bandicam-2021-06-15-13-51-38-131.jpg)
    * 스프링 컨테이너는 파라미터로 넘어온 설정 클래스 정보를 사용해서 빈을 등록한다.
    * **빈 이름**
        * 빈 이름은 메서드 이름을 사용한다.
        * 빈 이름을 직접 부여할 수 도 있다.
            * `Bean(name = "memberService2")`
        * **주의!!!**
            * **빈 이름은 항상 다른 이름을 부여해야 한다.**
            * 같은 이름을 부여하면, 다른 빈이 무시되거나, 기존 빈을 덮어버리거나 서정에 따라 오류가 발생한다.


3. **스프링 빈 의존관계 설정 - 준비**
   ![](https://i.ibb.co/sCycrXh/bandicam-2021-06-15-13-53-55-410.jpg)

4. **스프링 빈 의존관계 설정 - 완료**
   ![](https://i.ibb.co/tZN4q7f/bandicam-2021-06-15-13-54-24-373.jpg)
    * 스프링 컨테이너는 설정 정보를 참고해서 의존관계를 주입(DI)한다.
    * 단순히 자바 코드를 호출하는 것 같지만, 차이가 있다. 이 차이는 뒤에 싱글톤 컨테이너에서 설명한다.

> 참고    
> 스프링은 빈을 생성하고, 의존관계를 주입하는 단계가 나누어져 있다. 그런데 이렇게 자바 코드로 스프링 빈을 등록하면 생성자를 호출하면서 의존관계 주입도 한번에 처리된다. 여기서는 이해를 돕기 위해 개념적으로 나누어 설명했다. 자세한 내용은 의존관계 자동 주입에서 다시 설명한다.

> 정리        
> 스프링 컨테이너를 생성하고, 설정(구성)정보를 참고해서 스프링 빈도 등록하고, 의존관계도 설정했다.

### 4-2. 컨테이너에 등록된 모든 빈 조회

#### ApplicationContextInfoTest.java - 스프링 빈 조회

* `src/test/java/hello/core1/beanfind/ApplicationContextInfoTest.java`

```java
package hello.core1.beanfind;

import hello.core1.AppConfig;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

class ApplicationContextInfoTest {

    AnnotationConfigApplicationContext ac = new AnnotationConfigApplicationContext(AppConfig.class);

    @Test
    @DisplayName("모든 빈 출력하기")
    void findAllBean() {
        String[] beanDefinitionNames = ac.getBeanDefinitionNames();
        for (String beanDefinitionName : beanDefinitionNames) {
            Object bean = ac.getBean(beanDefinitionName);
            System.out.println("name = " + beanDefinitionName + " object = " + bean);
        }
    }

    @Test
    @DisplayName("애플리케이션 빈 출력하기")
    void findApplicationBean() {
        String[] beanDefinitionNames = ac.getBeanDefinitionNames();
        for (String beanDefinitionName : beanDefinitionNames) {
            BeanDefinition beanDefinition = ac.getBeanDefinition(beanDefinitionName);

            // Role ROLE_APPLICATION: 직접 등록한 애플리케이션 빈
            // Role ROLE_INFRASTRUCTURE: 스프링이 내부에서 사용하는 빈
            if (beanDefinition.getRole() == BeanDefinition.ROLE_APPLICATION) {
                Object bean = ac.getBean(beanDefinitionName);
                System.out.println("name = " + beanDefinitionName + " object = " + bean);
            }
        }
    }
}

```

* 모든 빈 출력하기
    * 실행하면 스프링에 등록된 모든 빈 정보를 출력할 수 있다.
    * `ac.geBeanDefinitionNames()`: 스프링에 등록된 모든 빈 이름을 조회한다.
    * `ac.getBean()`: 빈 이름으로 빈 객체(인스턴스)를 조회한다.

* 애플리케이션 빈 출력하기
    * 스프링이 내부에서 사용하는 빈은 제외하고, 내가 등록한 빈만 출력한다.
    * 스프링이 내부에서 사용하는 빈은 `getRole()`로 구분할 수 있다.
        * `ROLE_APPLICATION`: 일반적으로 사용자가 정의한 빈
        * `ROLE_INFRASTRUCTURE`: 스프링이 내부에서 사용하는 빈

### 4-3. 스프링 빈 조회 - 기본

스프링 컨테이너에서 스프링 빈을 찾는 가장 기본적인 조회 방법

* `ac.getBean(빈 이름, 타입)`
* `ac.getBean(타입)`
* 조회 대상 스프링 빈이 없으면 예외 발생
    * `NoSuchBeanDefinitionException: No Bean named 'xxxxx' available`

#### ApplicationContextBasicFindTest.java

* `src/test/java/hello/core1/beanfind/ApplicationContextBasicFindTest.java`

```java
package hello.core1.beanfind;

import hello.core1.AppConfig;
import hello.core1.member.MemberService;
import hello.core1.member.MemberServiceImpl;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.NoSuchBeanDefinitionException;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import static org.assertj.core.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertThrows;

class ApplicationContextBasicFindTest {

    AnnotationConfigApplicationContext ac = new AnnotationConfigApplicationContext(AppConfig.class);

    @Test
    @DisplayName("빈 이름으로 조회")
    void findBeanByName() {
        MemberService memberService = ac.getBean("memberService", MemberService.class);
        assertThat(memberService).isInstanceOf(MemberServiceImpl.class);
    }

    @Test
    @DisplayName("이름 없이 타입으로만 조회")
    void findBeanByType() {
        MemberService memberService = ac.getBean(MemberService.class);
        assertThat(memberService).isInstanceOf(MemberServiceImpl.class);
    }

    @Test
    @DisplayName("구체 타입으로 조회")
    void findBeanByName2() {
        MemberService memberService = ac.getBean("memberService", MemberServiceImpl.class);
        assertThat(memberService).isInstanceOf(MemberServiceImpl.class);
    }

    @Test
    @DisplayName("빈 이름으로 조회X")
    void findBeanByNameX() {
        // MemberService xxxxx = ac.getBean("xxxxx", MemberService.class);
        assertThrows(NoSuchBeanDefinitionException.class,
                () -> ac.getBean("xxxxx", MemberService.class));
    }
}

```

> 참고        
> 구체 타입으로 조회하면 변경시 유연성이 떨어진다.

### 4-4. 스프링 빈 조회 - 동일한 타입이 둘 이상

* 타입으로 조회시 같은 타입의 스프링 빈이 둘 이상이면 오류가 발생한다. 이때는 빈 이름을 지정하면 된다.
* `ac.getBeansOfTYpe()`을 사용하면 해당 타입의 모든 빈을 조회할 수 있다.

#### ApplicationContextSameBeanFindTest.java

* `src/test/java/hello/core1/beanfind/ApplicationContextSameBeanFindTest.java`

```java
package hello.core1.beanfind;

import hello.core1.member.MemberRepository;
import hello.core1.member.MemoryMemberRepository;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.NoUniqueBeanDefinitionException;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.util.Map;

import static org.assertj.core.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.*;

public class ApplicationContextSameBeanFindTest {

    AnnotationConfigApplicationContext ac = new AnnotationConfigApplicationContext(SameBeanConfig.class);

    @Test
    @DisplayName("타입으로 조회시 같은 타입이 둘 이상 있으면, 중복 오류가 발생한다.")
    void findBeanByTypeDuplication() {
        // MemberRepository bean = ac.getBean(MemberRepository.class);
        assertThrows(NoUniqueBeanDefinitionException.class,
                () -> ac.getBean(MemberRepository.class));
    }

    @Test
    @DisplayName("타입으로 조회시 같은 타입이 둘 이상 있으면, 빈 이름을 지정하면 된다.")
    void findBeanByName() {
        MemberRepository memberRepository = ac.getBean("memberRepository1", MemberRepository.class);
        assertThat(memberRepository).isInstanceOf(MemberRepository.class);
    }

    @Test
    @DisplayName("특정 타입을 모두 조회하기")
    void findAllBeanByType() {
        Map<String, MemberRepository> beansOfType = ac.getBeansOfType(MemberRepository.class);
        for (String key : beansOfType.keySet()) {
            System.out.println("key = " + key + " value = " + beansOfType.get(key));
        }
        System.out.println("beansOfType = " + beansOfType);
        assertThat(beansOfType.size()).isEqualTo(2);
    }

    @Configuration
    static class SameBeanConfig {

        @Bean
        public MemberRepository memberRepository1() {
            return new MemoryMemberRepository();
        }

        @Bean
        public MemberRepository memberRepository2() {
            return new MemoryMemberRepository();
        }
    }
}

```

### 4-5. 스프링 빈 조회 - 상속 관계

* 부모타입으로 조회하면, 자식 타입도 함께 조회한다.
* 그래서 모든 자바 객체의 최고 부모인 `Obejct`타입으로 조회하면, 모든 스프링 빈을 조회한다.

#### ApplicationContextExtendsFindTest.java

* `src/test/java/hello/core1/beanfind/ApplicationContextExtendsFindTest.java`

```java
package hello.core1.beanfind;

import hello.core1.discount.DiscountPolicy;
import hello.core1.discount.FixDiscountPolicy;
import hello.core1.discount.RateDiscountPolicy;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.NoUniqueBeanDefinitionException;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.util.Map;

import static org.assertj.core.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.*;

public class ApplicationContextExtendsFindTest {

    AnnotationConfigApplicationContext ac = new AnnotationConfigApplicationContext(TestConfig.class);

    @Test
    @DisplayName("부모 타입으로 조회시, 자식이 둘 이상 있으면, 중복 오류가 발생한다.")
    void findBeanByParentTypeDuplicate() {
        assertThrows(NoUniqueBeanDefinitionException.class,
                () -> ac.getBean(DiscountPolicy.class));
    }

    @Test
    @DisplayName("부모 타입으로 조회시, 자식이 둘 이상 있으면, 빈 이름을 지정하면 된다.")
    void findBeanByParentTypeBeanName() {
        DiscountPolicy rateDiscountPolicy = ac.getBean("rateDiscountPolicy", DiscountPolicy.class);
        assertThat(rateDiscountPolicy).isInstanceOf(RateDiscountPolicy.class);
    }

    @Test
    @DisplayName("특정 하위 타입으로 조회")
    void findBeanBySubType() {
        RateDiscountPolicy bean = ac.getBean(RateDiscountPolicy.class);
        assertThat(bean).isInstanceOf(RateDiscountPolicy.class);
    }

    @Test
    @DisplayName("부모 타입으로 모두 조회하기")
    void findAllBeanByParentType() {
        Map<String, DiscountPolicy> beansOfType = ac.getBeansOfType(DiscountPolicy.class);
        assertThat(beansOfType.size()).isEqualTo(2);
        for (String key : beansOfType.keySet()) {
            System.out.println("key = " + key + " value = " + beansOfType.get(key));
        }
    }

    @Test
    @DisplayName("부모 타입으로 모두 조회하기 - Object")
    void findALlBeansByObjectType() {
        Map<String, Object> beansOfType = ac.getBeansOfType(Object.class);
        for (String key : beansOfType.keySet()) {
            System.out.println("key = " + key + " value = " + beansOfType.get(key));
        }
    }

    @Configuration
    static class TestConfig {

        @Bean
        public DiscountPolicy rateDiscountPolicy() {
            return new RateDiscountPolicy();
        }

        @Bean
        public DiscountPolicy fixDiscountPolicy() {
            return new FixDiscountPolicy();
        }
    }
}

```

### 4-6. BeanFactory와 ApplicationContext

![](https://i.ibb.co/qrCbS9r/bandicam-2021-06-16-13-12-30-306.jpg)

* BeanFactory
    * 스프링 컨테이너의 최상위 인터페이스다.
    * 스프링 빈을 관리하고 조회하는 역할을 담당한다.
    * `getBean()`을 제공한다.
    * 지금까지 사용했던 대부분의 기능은 BeanFactory가 제공하는 기능이다.

* ApplicationContext
    * `BeanFactory`기능을 모두 상속받아서 제공한다.
    * 빈을 관리하고 검색하는 기능을 `BeanFactory`가 제공해주는데, 그러면 둘의 차이가 뭘까?
    * 애플리케이션을 개발할 때는 빈은 관리하고 조회하는 기능은 물론이고, 수 많은 부가기능이 필요하다.

#### ApplicationContext가 제공하는 부가기능

![](https://i.ibb.co/VDYWpwx/bandicam-2021-06-16-13-15-24-340.jpg)

* 메시지소스를 활용한 국제화 기능
    * 예를 들어서 한국에서 들어오면 한국어로, 영어권에서 들어오면 영어로 출력

* 환경변수
    * 로컬, 개발, 운영등을 구분해서 처리

* 애플리케이션 이벤트
    * 이벤트를 발행하고 구독하는 모델을 편리하게 지원

* 편리한 리소스 조회
    * 파일, 클래스패스, 외부 등에서 리소스를 편리하게 조회

#### 정리

* `ApplicationContext`는 `BeanFactory`의 기능을 상속받는다.
* `ApplicationContext`는 빈 관리기능 + 편리한 부가 기능을 제공한다.
* `BeanFactory`를 직접 사용할 일은 거의 없다. 부가기능이 포함된 `ApplicationContext`를 사용한다.
* `BeanFactory`나 `ApplicationContext`를 스프링 컨테이너라 한다.

### 4-7. 다양한 설정 형식 지원 - 자바 코드, XML

* 스프링 컨테이너는 다양한 형식의 설정 정보를 받아드릴 수 있게 유연하게 설계되어 있다.
* 자바 코드, XML, Groovy등등

![](https://i.ibb.co/VDYWpwx/bandicam-2021-06-16-13-15-24-340.jpghttps://i.ibb.co/27Cvd7H/bandicam-2021-06-16-13-34-05-196.jpg)

* 애노테이션 기반 자바 코드 설정 사용
    * 지금까지 했던 것
    * `new AnnotationConfigApplicationContext(AppConfig.class)`
    * `AnnotationConfigApplicationContext`클래스를 사용하면서 자바 코드로된 설정 정보를 넘기면 된다.

* XML 설정 사용
    * 최근에는 스프링 부트를 많이 사용하면서 XML기반의 설정은 잘 사용하지 않는다. 아직 많은 레거시 프로젝트들이 XML로 되어 있고, 또 XML을 사용하면 컴파일 없이 빈 설정 정보를 변경할 수 있는 단점도
      있으므로 한번쯤 배워두는 것도 괜찮다.
    * `GenericXmlApplicationContext`를 사용하면서 `xml`설정 파일을 넘기면 된다.

#### XmlAppContext.java - XmlAppConfig사용

* `src/test/java/hello/core1/xml/XmlAppContext.java`

```java
package hello.core1.xml;

import hello.core1.member.MemberService;
import org.junit.jupiter.api.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.GenericXmlApplicationContext;

import static org.assertj.core.api.Assertions.*;

public class XmlAppContext {

    @Test
    void xmlAppContext() {
        ApplicationContext ac = new GenericXmlApplicationContext("appConfig.xml");
        MemberService memberService = ac.getBean("memberService", MemberService.class);
        assertThat(memberService).isInstanceOf(MemberService.class);
    }
}

```

#### appConfig.xml - xml 기반의 스프링 빈 설정 정보

* `src/main/resources/appConfig.xml`

```html
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
    <bean id="memberService" class="hello.core1.member.MemberServiceImpl">
        <constructor-arg name="memberRepository" ref="memberRepository"/>
    </bean>
    <bean id="memberRepository"
          class="hello.core1.member.MemoryMemberRepository"/>
    <bean id="orderService" class="hello.core1.order.OrderServiceImpl">
        <constructor-arg name="memberRepository" ref="memberRepository"/>
        <constructor-arg name="discountPolicy" ref="discountPolicy"/>
    </bean>
    <bean id="discountPolicy" class="hello.core1.discount.RateDiscountPolicy"/>
</beans>
```

* xml 기반의 `appConfig.xml`스프링 설정 정보와 자바 코드로 된 `AppConfig.java`설정 정보를 비교해보면 거의 비슷하다는 것을 알 수 있다.
* xml 기반으로 설정하는 것은 최근에 잘 사용하지 않는다.

### 4-8. 스프링 빈 설정 메타 정보 - BeanDefinition

* 스프링은 어떻게 이런 다양한 설정 형식을 지원하는 것일까? 그 중심에는 `BeanDefinition`이라는 추상화가 있다.
* 쉽게 이야기해서 **역할과 구현을 개념적으로 나눈 것**이다!
    * XML을 읽어서 BeanDefinition을 만들면 된다.
    * 자바 코드를 읽어서 BeanDefinition을 만들면 된다.
    * 스프링 컨테이너는 자바 코드인지, XML인지 몰라도 된다. 오직 BeanDefinition만 알면 된다.
* `BeanDefintion`을 빈 설정 메타정보라 한다.
    * `@Bean`, `<bean>`당 각각 하나씩 메타 정보가 생성된다.
* 스프링 컨테이너는 이 메타정보를 기반으로 스프링 빈을 생성한다.

![](https://i.ibb.co/Vw7bXGX/bandicam-2021-06-16-14-02-18-317.jpg)

![](https://i.ibb.co/bvxQFBH/bandicam-2021-06-16-14-02-58-357.jpg)

* `AnnotationConfigApplicationContext`는 `AnnotatedBeanDefinitionReader`를 사용해서 `AppConfig.class`를 읽고 `BeanDefinition`을
  생성한다.
* `GenericXmlApplicationContext`는 `XmlBeanDefinitionReader`를 사용해서 `appConfig.xml`설정 정보를 읽고 `BeanDefinition`을 생성한다.
* 새로운 형식의 설정 정보가 추가되면, XxxBeanDefinitionReader를 만들어서 `BeanDefinition`을 생성한다.

#### BeanDefinition 정보

* BeanClassName: 생성할 빈의 클래스 명(자바 설정 처럼 팩토리 역할의 빈을 사용하면 없음)
* factoryBeanName: 팩토리 역할의 빈을 사용할 경우 이름, 예)appConfig
* factoryMethodName: 빈을 생성할 팩토리 메서드 지정, 예) memberService
* Scope: 싱글톤(기본값)
* lazyInit: 스프링 컨테이너를 생성할 때 빈을 생성하는 것이 아니라, 실제 빈을 사용할 때 까지 최대한 생성을 지연처리 하는지 여부
* InitMethodName: 빈을 생성하고, 의존관계를 적용한 뒤에 호출되는 초기화 메서드 명
* DestroyMethodName: 빈의 생명주기가 끝나서 제거하기 직전에 호출되는 메서드 명
* Constructor arguments, Properties: 의존관계 주입에서 사용한다. (자바 설정 처럼 패고리 역할의 빈을 사용하면 없음)

#### BeanDefinitionTest.java

* `src/test/java/hello/core1/beanDefinition/BeanDefinitionTest.java`

```java
package hello.core1.beanDefinition;

import hello.core1.AppConfig;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class BeanDefinitionTest {

    AnnotationConfigApplicationContext ac = new AnnotationConfigApplicationContext(AppConfig.class);

    @Test
    @DisplayName("빈 설정 메타정보 확인")
    void findApplicationBean() {
        String[] beanDefinitionNames = ac.getBeanDefinitionNames();
        for (String beanDefinitionName : beanDefinitionNames) {
            BeanDefinition beanDefinition = ac.getBeanDefinition(beanDefinitionName);

            if (beanDefinition.getRole() == BeanDefinition.ROLE_APPLICATION) {
                System.out.println("beanDefinitionName = " + beanDefinitionName +
                        " beanDefinition = " + beanDefinition);
            }
        }
    }
}

```

#### 정리

* BeanDefinition을 직접 생성해서 스프링 컨테이너에 등록할 수 도 있다. 하지만 실무에서 BeanDefinition을 직접 정의하거나 사용할 일은 거의 없다.
* BeanDefinition에 대해서는 너무 깊이있게 이해하기 보다는, 스프링이 다양한 형태의 설정 정보를 BeanDefinition으로 추상화해서 사용하는 것 정도만 이해하면 된다.
* 가끔 스프링 코드나 스프링 관련 오픈 소스의 코드를 볼 때, BeanDefinition 이라는 것이 보일 때가 있다. 이때 이러한 메커니즘을 떠올리면 된다.

## 5. 싱글톤 컨테이너

### 5-1. 웹 애플리케이션과 싱글톤

* 스프링은 태생이 기업용 온라인 서비스 기술을 지원하기 위해 탄생했다.
* 대부분의 스프링 애플리케이션은 웹 애플리케이션이다. 물론 웹이 아닌 애플리케이션 개발도 얼마든지 개발할 수 있다.
* 웹 애플리케이션은 보통 여러 고객이 동시에 요청을 한다.

![](https://i.ibb.co/6mnJWzZ/bandicam-2021-06-16-17-04-06-664.jpg)

#### SingletonTest.java - 스프링 없는 순수한 DI 컨테이너 테스트

* `src/test/java/hello/core1/singleton/SingletonTest.java`

```java
package hello.core1.singleton;

import hello.core1.AppConfig;
import hello.core1.member.MemberService;
import org.assertj.core.api.Assertions;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

public class SingletonTest {

    @Test
    @DisplayName("스프링 없는 순서한 DI 컨테이너")
    void pureContainer() {
        AppConfig appConfig = new AppConfig();
        // 1. 조회: 호출할 때 마다 객체를 생성
        MemberService memberService1 = appConfig.memberService();

        // 2. 조회: 호출할 때 마다 객체를 생성
        MemberService memberService2 = appConfig.memberService();

        // 참조값이 다른 것을 확인
        System.out.println("memberService1 = " + memberService1);
        System.out.println("memberService2 = " + memberService2);

        // memberService1 != memberService2
        Assertions.assertThat(memberService1).isNotSameAs(memberService2);
    }
}

```

* 우리가 만들었던 스프링 없는 순수한 DI 컨테이너인 AppConfig는 요청을 할 때 마다 객체를 새로 생성한다.
* 고객 트래픽이 초당 100이 나오면 초당 100개 객체가 생성되고 소멸된다. -> 메모리 낭비가 심하다.
* 해결방안은 해당 객체가 딱 1개만 생성되고, 공유하도록 설계하면 된다. -> 싱글톤 패턴

### 5-2. 싱글톤 패턴

* 클래스의 인스턴스가 딱 1개만 생성되는 것을 보장하는 디자인 패턴이다.
* 그래서 객체 인스턴스를 2개 이상 생성하지 못하도록 막아야 한다.
    * private 생성자를 사용해서 외부에서 임의로 new 키워드를 사용하지 못하도록 막아야 한다.

#### SingletonService.java - 싱글톤 패턴을 적용한 예제 코드

* `src/test/java/hello/core1/singleton/SingletonService.java`

```java
package hello.core1.singleton;

public class SingletonService {

    private static final SingletonService instance = new SingletonService();

    public static SingletonService getInstance() {
        return instance;
    }

    private SingletonService() {
    }

    public void logic() {
        System.out.println("싱글톤 객체 로직 호출");
    }
}

```

1. static 영역에 객체 `instance`를 미리 하나 생성해서 올려둔다.
2. 이 객체 인스턴스가 필요하면 오직 `getInstacne()`메서드를 통해서만 조회할 수 있다. 이 메서드를 호출하면 항상 같은 인스턴스를 반환한다.
3. 딱 1개의 객체 인스턴스만 존재해야 하므로, 생성자를 `private`으로 막아서 혹시라도 외부에서 new 키워드로 객체 인스턴스가 생성되는 것을 막는다.

#### SingletonTest.java(수정) - 싱글톤 패턴 테스드

```java
package hello.core1.singleton;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.assertj.core.api.Assertions.*;

public class SingletonTest {

    @Test
    @DisplayName("싱글톤 패턴을 적용한 객체 사용")
    void singletonServiceTest() {
        SingletonService singletonService1 = SingletonService.getInstance();
        SingletonService singletonService2 = SingletonService.getInstance();

        System.out.println("singletonService1 = " + singletonService1);
        System.out.println("singletonService2 = " + singletonService2);

        assertThat(singletonService1).isSameAs(singletonService2);
    }
}

```

* pirvate 으로 new 키워들 막아두었다.
* 호출할 때 마다 같은 객체 인스턴스를 반환하는 것을 확인할 수 있다.

> 참고    
> 싱글톤 패턴을 구현하는 방법은 여러가지가 있다. 여기서는 객체를 미리 생성해두는 가장 단순하고 안전한 방법을 선택했다.

싱글톤 패턴을 적용하면 고객의 요청이 올 때 마다 객체를 생성하는 것이 아니라, 이미 만들어진 객체를 공유해서 효율적으로 사용할 수 있다. 하지만 싱글톤 패턴은 다음과 같은 수 많은 문제점들을 가지고 있다.

#### 싱글톤 패턴 문제점

* 싱글톤 패턴을 구현하는 코드 자체가 많이 들어간다.
* 의존관계상 클라이언트가 구체 클래스에 의존한다. -> DIP를 위반한다.
* 클라이언트가 구체 클래스에 의존해서 OCP 원칙을 위반할 가능성이 높다.
* 테스트하기 어렵다.
* 내부 속성을 변경하거나 초기화 하기 어렵다.
* private 생성자로 자식 클래스를 만들기 어렵다.
* 결론적으로 유연성이 떨어진다.
* 안티패턴으로 불리기도 한다.

### 5-3. 싱글톤 컨테이너

스프링 컨테이너는 싱글톤 패턴의 문제점을 해결하면서, 객체 인스턴스를 싱글톤(1개만 생성)으로 관리한다.      
지금까지 우리가 학습한 스프링 빈이 바로 싱글톤으로 관리되는 빈이다.

* 싱글톤 컨테이너
    * 스프링 컨테이너는 싱글톤 패턴을 적용하지 않아도, 객체 인스턴스를 싱글톤으로 관리한다.
        * 이전에 설명한 컨테이너 생성 과정을 자세히 보자. 컨테이너는 객체를 하나만 생성해서 관리한다.
    * 스프링 컨테이너는 싱글톤 컨테이너 역할을 한다. 이렇게 싱글톤 객체를 생성하고 관리하는 기능을 싱글톤 레지스트리라 한다.
    * 스프링 컨테이너의 이런 기능 덕분에 싱글턴 패턴의 모든 단점을 해결하면서 객체를 싱글톤으로 유지할 수 있다.
        * 싱글톤 패턴을 위한 지저분한 코드가 들어가지 않아도 된다.
        * DIP, OCP, 테스트, private 생성자로 부터 자유롭게 싱글톤을 사용할 수 있다.

#### SingletonTest(추가) - 스프링 컨테이너를 사용하는 테스트 코드

```java
public class SingletonTest {
    @Test
    @DisplayName("스프링 컨테이너와 싱글톤")
    void springContainer() {
        ApplicationContext ac = new AnnotationConfigApplicationContext(AppConfig.class);

        // 1. 조회: 호출할 때 마다 객체를 생성
        MemberService memberService1 = ac.getBean("memberService", MemberService.class);

        // 2. 조회: 호출할 때 마다 객체를 생성
        MemberService memberService2 = ac.getBean("memberService", MemberService.class);

        // 참조값이 같은 것을 확인
        System.out.println("memberService1 = " + memberService1);
        System.out.println("memberService2 = " + memberService2);

        // memberService1 == memberService2
        assertThat(memberService1).isSameAs(memberService2);
    }
}
```

#### 싱글톤 컨테이너 적용 후

![](https://i.ibb.co/2YRwZMg/bandicam-2021-06-17-10-54-35-353.jpg)

* 스프링 컨테이너 덕분에 고객의 요청이 올 때 마다 객체를 생성하는 것이 아니라, 이미 만들어진 객체를 공유해서 효율적으로 재사용할 수 있다.

> 참고        
> 스프링의 기본 빈 등록 방식은 싱글톤이지만, 싱글톤 방식만 지원하는 것은 아니다. 요청할 때 마다 새로운 객체를 생성해서 반환하는 기능도 제공한다.

### 5-4. 싱글톤 방식의 주의점

* 싱글톤 패턴이든, 스프링 같은 싱글톤 컨테이너를 사용하든, 객체 인스턴스를 하나만 생성해서 공유하는 싱글톤 방식은 여러 클라이언트가 하나의 같은 객체 인스턴스를 공유하기 때문에 싱글톤 객체는 상태를 유지(
  stateful)하게 설계하면 안된다.
* 무상태(stateless)로 설계해야 한다.
    * 특정 클라이언트에 의존적인 필드가 있으면 안된다.
    * 특정 클라이언트가 값을 변경할 수 있는 필드가 있으면 안된다.
    * 가급적 읽기만 가능해야 한다.
    * 필드 대신에 자바에서 공유되지 않는, 지역변수, 파라미터, ThreadLocal등을 사용해야 한다.

* 스프링 빈의 필드에 공유 값을 설정하면 정말 큰 장애가 발생할 수 있다.

#### StatefulService.java - 상태를 유지할 경우 발생하는 문제점 예시

* `src/test/java/hello/core1/singleton/StatefulService.java`

```java
package hello.core1.singleton;

public class StatefulService {

    private int price; // 상태를 유지하는 필드

    public void order(String name, int price) {
        System.out.println("name = " + name + " price = " + price);
        this.price = price;
    }

    public int getPrice() {
        return price;
    }
}

```

#### StatefulService.java - 상태를 유지할 경우 발생하는 문제점 예시

* `src/test/java/hello/core1/singleton/StatefulServiceTest.java`

```java
package hello.core1.singleton;

import org.junit.jupiter.api.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Bean;

import static org.assertj.core.api.Assertions.*;

public class StatefulServiceTest {

    @Test
    void statefulServiceSingleton() {
        ApplicationContext ac = new AnnotationConfigApplicationContext(TestConfig.class);
        StatefulService statefulService1 = ac.getBean(StatefulService.class);
        StatefulService statefulService2 = ac.getBean(StatefulService.class);

        // ThreadA: A사용자 10000원 주문
        statefulService1.order("userA", 10000);

        // ThreadB: 사용자B 20000원 주문
        statefulService2.order("userB", 20000);

        // ThreadA: 사용자A 주문 금액 조회
        int price = statefulService1.getPrice();
        System.out.println("price = " + price);

        assertThat(statefulService1.getPrice()).isEqualTo(20000);
    }

    static class TestConfig {

        @Bean
        public StatefulService statefulService() {
            return new StatefulService();
        }
    }
}
```

* 최대한 단순히 설명하기 위해, 실제 쓰레드는 사용하지 않았다.
* ThreadA가 사용자A 코드를 호출하고 ThreadB가 사용자 B 코드를 호출한다 가정하다.
* `StatefulService`의 `price`필드는 공유되는 필드인데, 특정 클라이언트가 값을 변경한다.
* 사용자A의 주문금액은 10000원이 되어야 하는데, 20000원이라는 결과가 나왔다.
* 실무에서 이런 경우를 종종 보는데, 이로인해 정말 해결하기 어려운 큰 문제들이 터진다.
* 진짜 공유필드는 조심해야 한다! 스프링 빈은 항상 무상태(stateless)로 설계하자.

### 5-5. @Configuration과 싱글톤

`AppConfig.java` 코드를 살펴보자

```java

@Configuration
public class AppConfig {
    @Bean
    public MemberService memberService() {
        return new MemberServiceImpl(memberRepository());
    }

    @Bean
    public OrderService orderService() {
        return new OrderServiceImpl(
                memberRepository(),
                discountPolicy());
    }

    @Bean
    public MemberRepository memberRepository() {
        return new MemoryMemberRepository();
    }
 ...
}
```

* `memberService`빈을 만드는 코드를 보면 `memberRepository()`를 호출한다.
    * 이 메서드를 호출하면 `new MemoryMemberRepository()`를 호출한다.

* `orderService` 빈을 만드는 코드도 동일하게 `memberRepository()`를 호출한다.
    * 이 메서드를 호출하면 `new MemroyMemberRepository()`를 호출한다.

결과적으로 각각 다른 2개의 `MomoryMemberRepository`가 생성되면서 싱글톤이 깨지는 것 처럼 보인다. 스프링 컨테이너는 이 문제를 어떻게 해결할까?

#### MemberServiceImpl.java, OrderServiceImpl.java - 검증 용도 코드 추가

```java
public class MemberServiceImpl implements MemberService {
    private final MemberRepository memberRepository;

    //테스트 용도
    public MemberRepository getMemberRepository() {
        return memberRepository;
    }
}

public class OrderServiceImpl implements OrderService {
    private final MemberRepository memberRepository;

    //테스트 용도
    public MemberRepository getMemberRepository() {
        return memberRepository;
    }
}
```

#### ConfigurationSingletonTest.java - 테스트 코드

* `src/test/java/hello/core1/singleton/ConfigurationSingletonTest.java`

```java
package hello.core1.singleton;

import hello.core1.AppConfig;
import hello.core1.member.MemberRepository;
import hello.core1.member.MemberServiceImpl;
import hello.core1.order.OrderServiceImpl;
import org.junit.jupiter.api.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import static org.assertj.core.api.Assertions.*;

public class ConfigurationSingletonTest {

    @Test
    void configurationTest() {
        ApplicationContext ac = new AnnotationConfigApplicationContext(AppConfig.class);

        MemberServiceImpl memberService = ac.getBean("memberService", MemberServiceImpl.class);
        OrderServiceImpl orderService = ac.getBean("orderService", OrderServiceImpl.class);
        MemberRepository memberRepository = ac.getBean("memberRepository", MemberRepository.class);

        MemberRepository memberRepository1 = memberService.getMemberRepository();
        MemberRepository memberRepository2 = orderService.getMemberRepository();

        System.out.println("memberService -> memberRepository1 = " + memberRepository1);
        System.out.println("orderService -> memberRepository2 = " + memberRepository2);
        System.out.println("memberRepository = " + memberRepository);

        assertThat(memberService.getMemberRepository()).isSameAs(memberRepository);
        assertThat(orderService.getMemberRepository()).isSameAs(memberRepository);
    }
}

```

* 확인해보면 `memberRepository`인스턴스는 모두 같은 인스턴스가 공유되어 사용된다.
* `AppConfig`의 자바 코드를 보면 분명히 각각 2번 `new MemoryMemberRepository`호출해서 다른 인스턴스가 생성되어야 하는데?
* 어떻게 된 일일까? 혹시 두 번 호출이 안되는 것일까? 실험을 통해 알아보자.

#### AppConfig.java(추가) - 호출 로그 남김

```java
package hello.core;

import hello.core.discount.DiscountPolicy;
import hello.core.discount.RateDiscountPolicy;
import hello.core.member.MemberRepository;
import hello.core.member.MemberService;
import hello.core.member.MemberServiceImpl;
import hello.core.member.MemoryMemberRepository;
import hello.core.order.OrderService;
import hello.core.order.OrderServiceImpl;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class AppConfig {
    @Bean
    public MemberService memberService() {
        //1번
        System.out.println("call AppConfig.memberService");
        return new MemberServiceImpl(memberRepository());
    }

    @Bean
    public OrderService orderService() {
        //1번
        System.out.println("call AppConfig.orderService");
        return new OrderServiceImpl(
                memberRepository(),
                discountPolicy());
    }

    @Bean
    public MemberRepository memberRepository() { //2번? 3번?
        System.out.println("call AppConfig.memberRepository");
        return new MemoryMemberRepository();
    }

    @Bean
    public DiscountPolicy discountPolicy() {
        return new RateDiscountPolicy();
    }
}
```

스프링 컨테이너가 각각 @Bean을 호출해서 스프링 빈을 생성한다. 그래서 `memberRepository()`는 다음과 같이 총 3번이 호출되어야 하는 것 아닐까?

1. 스프링 컨테이너가 스프링 빈에 등록하기 위해 @Bean이 붙어있는 `memberRepository()`호출
2. `memberService()`로직에서 `memberRepository()`호출
3. `orderService()`로직에서 `ememberRepository()`호출

그런테 출력 결과는 모두 1번만 호출된다.

```
call AppConfig.memberService
call AppConfig.memberRepository
call AppConfig.orderService
```

### 5-6. @Configuration과 바이터코드 조작의 마법

스프링 컨테이너는 싱글톤 레지스트리다. 따라서 스프링 빈이 싱글톤이 되도록 보장해주어야 한다. 그런데 스프링이 자바 코드까지 어떻게 하기는 어렵다. 저 자바 코드를 보면 분명 3번 호출되어야 하는 것이 맞다. 그래서
스프링은 클래스의 바이트코드를 조작하는 라이브러리를 사용한다.     
모든 비밀은 `@Configuration`을 적용한 `AppConfig`에 있다.

#### ConfigurationSingletonTest.java(추가)

```java
public class ConfigurationSingletonTest {
    @Test
    void configurationDeep() {
        ApplicationContext ac = new
                AnnotationConfigApplicationContext(AppConfig.class);

        //AppConfig도 스프링 빈으로 등록된다.
        AppConfig bean = ac.getBean(AppConfig.class);

        System.out.println("bean = " + bean.getClass());
        //출력: bean = class hello.core.AppConfig$$EnhancerBySpringCGLIB$$bd479d70
    }
}
```

* 사실 `AnnotationConfigApplicationContex`에 파라미터로 넘긴 값은 스프링 빈으로 등록된다. 그래서 `AppConfig`도 스프링 빈이 된다.
* `AppConfig`스프링 빈을 조회해서 클래스 정보를 출력해보자.

```
bean = class hello.core.AppConfig$$EnhancerBySpringCGLIB$$bd479d70
```

순수한 클래스라면 다음과 같이 출력되어야 한다.

```
class hello.core.AppConfig
```

그런데 예상과는 다르게 클래스 명에 xxxCGLIB가 붙으면서 상다히 복잡해진 것을 볼 수 있다. 이것은 내가 만든 클래스가 아니라 스프링이 CGLIB라는 바이트코드 조작 라이브러리를 사용해서 AppConfig
클래스를 상속받은 임의의 다른 클래스를 만들고, 그 다른 클래스를 스프링 빈으로 등록한 것이다.

![](https://i.ibb.co/Xj1y6SD/bandicam-2021-06-17-13-53-00-361.jpg)

그 임의의 다른 클래스가 바로 싱글톤이 보장되도록 해준다. 아마도 다음과 같이 바이트 코드를 조작해서 작성되어 있을 것이다.(실제로는 CGLIB의 내부 기술을 사용하는데 매우 복잡하다.)

#### AppConfig@CGLIB 예상 코드

```
public class AppConfig@CGLIB {
    @Bean
    public MemberRepository memberRepository() {

        if (memoryMemberRepository가 이미 스프링 컨테이너에 등록되어 있으면?) {
            return 스프링 컨테이너에서 찾아서 반환;
        } else { //스프링 컨테이너에 없으면
            기존 로직을 호출해서 MemoryMemberRepository를 생성하고 스프링 컨테이너에 등록
            return 반환
        }
    }
}
```

* @Bean이 붙은 메서드마다 이미 스프링 빈이 존재하면 존재하는 빈을 반환하고, 스프링 빈이 없으면 생성해서 스프링 빈으로 등록하고 반환하는 코드가 동적으로 만들어진다.
* 덕분에 싱글톤이 보장되는 것이다.

> 참고    
> AppConfig@CGLIB는 AppConfig의 자식타입이므로, AppConfig 타입으로 조회할 수 있다.

#### @Configuration 을 적용하지 않고, @Bean 만 적용하면 어떻게 될까?

`@Configuration` 을 붙이면 바이트코드를 조작하는 CGLIB 기술을 사용해서 싱글톤을 보장하지만, 만약 `@Bean`만 적용하면 어떻게 될까?

```
//@Configuration 삭제
public class AppConfig {
}
```

이제 똑같이 실행해보자.

```
bean = class hello.core.AppConfig
```

이 출력 결과를 통해서 `AppConfig`가 CGLIB 기술 없이 순수한 `AppConfig`로 스프링 빈에 등록된 것을 확인할 수 있다.

```
call AppConfig.memberService
call AppConfig.memberRepository
call AppConfig.orderService
call AppConfig.memberRepository
call AppConfig.memberRepository
```

이 출력 결과를 통해서 `MemberRepository`가 총 3번 호출된 것을 알 수 있다. 1번은 `@Bean`에 의해 스프링 컨테이너에 등록하기 위해서이고, 2번은 각각 `memberRepository()`
를 호출하면서 발생한 코드다.

* 인스턴스가 같은지 테스트 결과
    ```
    memberService -> memberRepository =
    hello.core.member.MemoryMemberRepository@6239aba6
    orderService -> memberRepository =
    hello.core.member.MemoryMemberRepository@3e6104fc
    memberRepository = hello.core.member.MemoryMemberRepository@12359a82
    ```

당연히 인스턴스가 같은지 테스트 하는 코드도 실패하고, 각각 다 다른 `MemoryMemberRepository` 인스턴스를 가지고 있다.확인이 끝났으면 `@Configuration`이 동작하도록 다시 돌려놓자.

#### 정리

* `@Bean`만 사용해도 스프링 빈으로 등록되지만, 싱글톤을 보장하지 않는다.
    * `memberRepository()` 처럼 의존관계 주입이 필요해서 메서드를 직접 호출할 때 싱글톤을 보장하지 않는다.
* 크게 고민할 것이 없다. 스프링 설정 정보는 항상 `@Configuration`을 사용하자.

## 6. 컴포넌트 스캔

### 6-1. 컴포넌트 스캔과 의존관계 자동 주입 시작하기

* 지금까지 스프링 빈을 등록할 때는 자바 코드는 `@Bean`이나 XML의 `<bean>`등을 통해서 설정 정보에 직접 등록할 스프링 빈을 나열했다.
* 예제에서는 몇개가 안되었지만, 이렇게 등록해야 할 스프링 빈이 수십, 수백개가 되면 일일이 등록하기도 귀찮고, 설정 정보도 커지고, 누락하는 문제도 발생한다.
* 그래서 스프링은 설정 정보가 없어도 자동으로 스프링 빈을 등록하는 컴포넌트 스캔이라는 기능을 제공한다.
* 또 의존관계도 자동으로 주입하는 `@Autowired`라는 기능도 제공한다.

#### AutoAppConfig.java

* `src/main/java/hello/core1/AutoAppConfig.java`

```java
package hello.core1;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;

@Configuration
@ComponentScan(
        excludeFilters = @ComponentScan.Filter(type = FilterType.ANNOTATION, classes = Configuration.class)
)
public class AutoAppConfig {

}

```

* 컴포넌트 스캔을 사용하려면 먼저 `@ComponentScan`을 설정 정보에 붙여주면 된다.
* 기존의 `AppConfig`와는 다르게 `@Bean`으로 등록한 클래스가 하나도 없다.

> 참고    
> 컴포넌트 스캔을 사용하면 `@Configuration`이 붙은 설정 정보도 자동으로 등록되기 때문에, `AppConfig`, `TestConfig`등 앞서 만들어두었던 설정 정보도 함께 등록되고, 실행되어 버린다. 그래서 `excludeFilters`를 이용해서 설정 정보는 컴포넌트 스캔 대상에서 제외했다. 보통 설정 정보를 컴포넌트 스캔 대상에서 제외하지는 않지만, 기존 예제 코드를 최대한 남기고유지하기 위해서 이 방법을 선택했다.

컴포넌트 스캔은 이름 그대로 `@Component`애노테이션이 붙은 클래스를 스캔해서 스프링 빈으로 등록한다. `@Component`를 붙여주자.

> 참고    
> `@Configuration`이 컴포넌트 스캔의 대상이 된 이유도 `@Configuration`소스코드를 열어보면 `@Component`애노테이션이 붙어있기 때문이다.

이제 각 클래스가 컴포넌트 스캔의 대상이 되도록 `@Component`애노테이션을 붙여준다.

#### MemoryMemberRepository.java(추가) - @Component 추가

```java

@Component
public class MemoryMemberRepository implements MemberRepository {
}
```

#### RateDiscountPolicy.java(추가) - @Component 추가

```java

@Component
public class RateDiscountPolicy implements DiscountPolicy {
}
```

#### MemberServiceImpl.java(추가) - @Component, @Autowired 추가

```java

@Component
public class MemberServiceImpl implements MemberService {
    private final MemberRepository memberRepository;

    @Autowired
    public MemberServiceImpl(MemberRepository memberRepository) {
        this.memberRepository = memberRepository;
    }
}
```

* 이전에 `AppConfig`에서는 `@Bean`으로 직접 설정 정보를 작성했고, 의존관계도 직접 명시했다. 이제는 이런 설정 정보 자체가 없기 때문에, 의존관계 주입도 이 클래스 안에서 해결해야 한다.
* `@Autowired`는 의존관계를 자동으로 주입해준다.

#### OrderServiceImpl.java (추가) - @Component, @Autowired 추가

```java

@Component
public class OrderServiceImpl implements OrderService {
    private final MemberRepository memberRepository;
    private final DiscountPolicy discountPolicy;

    @Autowired
    public OrderServiceImpl(MemberRepository memberRepository, DiscountPolicy
            discountPolicy) {
        this.memberRepository = memberRepository;
        this.discountPolicy = discountPolicy;
    }
}
```

* `@Autowired`를 사용하면 생성자에서 여러 의존관계도 한번에 주입받을 수 있다.

#### AutoAppConfigTest.java

* `src/test/java/hello/core1/scan/AutoAppConfigTest.java`

```java
package hello.core1.scan;

import hello.core1.AutoAppConfig;
import hello.core1.member.MemberService;
import org.junit.jupiter.api.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import static org.assertj.core.api.Assertions.*;

public class AutoAppConfigTest {

    @Test
    void basicScan() {
        AnnotationConfigApplicationContext ac = new AnnotationConfigApplicationContext(AutoAppConfig.class);

        MemberService memberService = ac.getBean(MemberService.class);
        assertThat(memberService).isInstanceOf(MemberService.class);
    }
}

```

* `AnootaionConfigApplicationContext`를 사용하는 것은 기존과 동일하다.
* 설정 정보로 `AutoAppConfig`클래스를 넘겨준다.
* 실행해보면 기존과 같이 잘 동작하는 것을 확인할 수 있다.
* 로그를 잘 보면 컴포넌트 스캔이 잘 동작하는 것을 확인할 수 있다.

```
ClassPathBeanDefinitionScanner - Identified candidate component class:
.. RateDiscountPolicy.class
.. MemberServiceImpl.class
.. MemoryMemberRepository.class
.. OrderServiceImpl.class
```

#### 컴포넌트 스캔과 자동 의존관계 주입

1. `@ComponentScan`
   ![](https://i.ibb.co/3hYWMTc/bandicam-2021-06-18-13-27-33-391.jpg)
    * `@ComponentScan`은 `@Component`가 붙은 모든 클래스를 스프링 빈으로 등록한다.
    * 이때 스프링 빈의 기본 이름은 클래스명을 사용하되 맨 앞글자만 소문자를 사용한다.
        * **빈 이름 기본 전략**: MemberServiceImpl 클래스 -> memberServiceImpl
        * **빈 이름 직접 지정**: 만약 스프링 빈의 이름을 직접 지정하고 싶으면 `@Component("memberService2")`이런식으로 이름을 부여하면 된다.


2. `@Autowired`의존관계 자동 주입
   ![](https://i.ibb.co/5WMBCyk/bandicam-2021-06-18-13-30-19-583.jpg)
    * 생성자에 `@Autowired`를 지정하면, 스프링 컨테이너가 자동으로 해당 스프링 빈을 찾아서 주입한다.
    * 이때 기본 조회 전략은 타입이 같은 빈을 찾아서 주입한다.
        * `getBean(MemberRepository.class)`와 동일하다고 이해하면 된다.

   ![](https://i.ibb.co/NVKz15d/bandicam-2021-06-18-13-32-07-710.jpg)
    * 생성자에 파라미터가 많아도 다 찾아서 자동으로 주입한다.

### 6-2. 탐색 위치와 기본 스캔 대상

#### 탐색할 패키지의 시작 위치 지정

모든 자바 클래스를 다 컴포넌트 스캔하면 시간이 오래 걸린다. 그래서 꼭 필요한 위치부터 탐색하도록 시작 위치를 지정할 수 있다.

```
@ComponentScan(
        basePackage = "hello.core1",
}
```

* `basePackage`: 탐색할 패키지의 시작 위치를 지정한다. 이 패키지를 포함해서 하위 패키지를 모두 탐색한다.
    * `basePackages = {"hello.core", "hello.service"}`이렇게 여러 시작 위치를 지정할 수도 있다.
* `basePackageClasses`: 지정한 클래스의 패키지를 탐색 시작 위치로 지정한다.
* 만약 지정하지 않으면 `@ComponentScan`이 붙은 설정 정보 클래스의 패키지가 시작 위치가 된다.

#### 권장하는 방법

개인적으로 즐겨 사용하는 방법은 패키지 위치를 지정하지 않고, 설정 정보 클래스의 위치를 프로젝트 최상단에 두는 것이다. 최근 스프링 부트도 이 방법을 기본으로 제공한다.

예를 들어서 프로젝트가 다음과 같이 구조가 되어 있으면

* `com.hello`
* `com.hello.service`
* `com.hello.repository`

`com.hello` -> 프로젝트 시작 루트, 여기에 `AppConfig`같은 메인 설정 정보를 두고, `@ComponentScan`애노테이션을 붙이고, `basePackages`지정은 생략한다.

이렇게 하면 `com.hello`를 포함한 하위는 모두 자동으로 컴포넌트 스캔의 대상이 된다. 그리고 프로젝트 메인 설정 정보는 프로젝트를 대표하는 정보이기 때문에 프로젝트 시작 루트 위치에 두는 것이 좋다
생각한다.       
참고로 스프링 부트를 사용하면 스프링 부트의 대표 시작 정보인 `@SpringBootApplication`를 이 프로젝트 시작 루트 위치에 두는 것이 관례이다. (그리고 이 설정안에
바로 `@ComponentScan`이 들어있다!)

#### 컴포넌트 스캔 기본 대상

컴포넌트 스캔은 `@Component`뿐만 아니라 다음과 내용도 추가로 대상에 포함한다.

* `@Component`: 컴포넌트 스캔에서 사용
* `@Controller`: 스프링 MVC 컨트롤러에서 사용
* `@Service`: 스프링 비즈니스 로직에서 사용

* `@Repository`: 스프링 데이터 접근 계층에서 사용
* `@Configuration`: 스프링 설정 정보에서 사용

해당 클래스의 소스 코드를 보면 `@Component`를 포함하고 있는 것을 알 수 있다.

```java

@Component
public @interface Controller {
}

@Component
public @interface Service {
}

@Component
public @interface Configuration {
}
```

> 참고    
> 사실 애노테이션에는 상속관계라는 것이 없다. 그래서 이렇게 애노테이션이 특정 애노테이션을 들고 있는 것을 인식할 수 있는 것은 자바 언어가 지원하는 기능은 아니고, 스프링이 지원하는 기능이다.

컴포넌트 스캔의 용도 뿐만 아니라 다음 애노테이션이 있으면 스프링은 부가 기능을 수행한다.

* `@Controller`: 스프링 MVC 컨트롤러로 인식
* `@Repository`: 스프링 데이터 접근 계층으로 인식하고, 데이터 계층의 예외를 스프링 예외로 변환해준다.
* `@Configuration`: 앞서 보았듯이 스프링 설정 정보로 인식하고, 스프링 빈이 싱글톤을 유지하도록 추가 처리를 한다.
* `@Service`: 사실 `@Service`는 특별한 처리를 하지 않는다. 대신 개발자들이 핵심 비즈니스 로직이 여기에 있겠구나 라고 비즈니스 계층을 인식하는데 도움이 된다.

> 참고    
> `useDefaultFilters`옵션은 기본으로 켜져있는데, 이 옵션을 끄면 기본 스캔 대상들이 제외된다. 그냥 이런 옵션이 있구나 정도 알고 넘어가자.

### 6-3. 필터

* `includeFilters`: 컴포넌트 스캔 대상을 추가로 지정한다.
* `excludeFilters`: 컴포넌트 스캔에서 제외할 대상을 지정한다.

#### MyIncludeComponent.java - 컴포넌트 스캔 대상에 추가할 애노테이션

* `src/test/java/hello/core1/scan/filter/MyIncludeComponent.java`

```java
package hello.core1.scan.filter;

import java.lang.annotation.*;

@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface MyIncludeComponent {

}

```

#### MyExcludeComponent.java - 컴포넌트 스캔 대상에 제외할 애노테이션

* `src/test/java/hello/core1/scan/filter/MyExcludeComponent.java`

```java
package hello.core1.scan.filter;

import java.lang.annotation.*;

@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface MyExcludeComponent {

}

```

#### BeanA.java - 컴포넌트 스캔 대상에 추가할 클래스

* `src/test/java/hello/core1/scan/filter/BeanA.java`

```java
package hello.core1.scan.filter;

@MyIncludeComponent
public class BeanA {
}

```

* `@MyIncludeComponent`적용

#### BeanB.java - 컴포넌트 스캔 대상에서 제외할 클래스

* `src/test/java/hello/core1/scan/filter/BeanB.java`

```java
package hello.core1.scan.filter;

@MyExcludeComponent
public class BeanB {
}

```

* `MyExcludeComponent`사용

#### ComponentFilterAppConfigTest - 설정 정보와 전체 테스트 코드

```java
package hello.core1.scan.filter;

import org.assertj.core.api.Assertions;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.NoSuchBeanDefinitionException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;

import static org.assertj.core.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.*;
import static org.springframework.context.annotation.ComponentScan.*;

public class ComponentFilterAppConfigTest {

    @Test
    void filterScan() {
        ApplicationContext ac = new AnnotationConfigApplicationContext(ComponentFilterAppConfig.class);
        BeanA beanA = ac.getBean("beanA", BeanA.class);
        assertThat(beanA).isNotNull();

        assertThrows(
                NoSuchBeanDefinitionException.class,
                () -> ac.getBean("beanB", BeanB.class)
        );
    }

    @Configuration
    @ComponentScan(includeFilters = @Filter(classes = MyIncludeComponent.class),
            excludeFilters = @Filter(classes = MyExcludeComponent.class))
    static class ComponentFilterAppConfig {

    }
}

```

* `includeFilters`에 `MyIncludeComponent`애노테이션을 추가해서 `BeanA`가 스프링 빈에 등록된다.
* `excludeFilters`에 `MyExcluldeComponent`애노테이션을 추가해서 `BeanB`가 스프링 빈에 등록되지 않는다.

#### FilterType 옵션

FilterType은 5가지 옵션이 있다.

* ANNOTATION: 기본값, 애노테이션을 인식해서 동작한다.
    * ex) `org.example.SomeAnnotation`
* ASSIGNABLE_TYPE: 지정한 타입과 자식 타입을 인식해서 동작한다.
    * ex) `org.example.SomeClass`
* ASPECTJ: AspectJ 패턴 사용
    * ex) `org.example..*Service+`
* REGEX: 정규 표현식
    * ex) `org\.example\.Default.*`
* CUSTOM: `TypeFilter`이라는 인터페이스를 구현해서 처리
    * ex) `org.example.MyTypeFilter`

예를 들어서 `BeanA`도 빼고 싶으면 다음과 같이 추가하면 된다.

```java
@ComponentScan(
        includeFilters = {
                @Filter(type = FilterType.ANNOTATION, classes =
                        MyIncludeComponent.class),
        },
        excludeFilters = {
                @Filter(type = FilterType.ANNOTATION, classes =
                        MyExcludeComponent.class),
                @Filter(type = FilterType.ASSIGNABLE_TYPE, classes = BeanA.class)
        }
)
```

> 참고    
> `@Component`면 충분하기 때문에, `includeFilters`를 사용할 일은 거의 없다. `excludeFilters`는 여러가지 이유로 간혹 사용할 때가 있지만 많지는 않다.      
> 특히 최근 스프링 부트는 컴포넌트 스캔을 기본으로 제공하는데, 개인적으로는 옵션을 변경하면서 사용하기 보다는 스프링의 기본 설정에 최대한 맞추어 사용하는 것을 권장하고, 선호하는 편이다.

## Note