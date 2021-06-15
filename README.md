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

## Note