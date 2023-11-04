import oguzhantopaloglu.data.DataManager;
import oguzhantopaloglu.data.QuestionSet;
import oguzhantopaloglu.data.QuestionSetOption;
import oguzhantopaloglu.Utils;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.Random;


public class Tests {

    @Test
    @DisplayName("Test1")
    void test1() {
        QuestionSet q = new QuestionSet("test", 1, 10, 200);
        Assertions.assertEquals(200, q.getLeftValues().length);
        Assertions.assertEquals(200, q.getRightValues().length);
        for (int i = 0; i < 200; i++) {
            Assertions.assertTrue(1 <= q.getLeftValues()[i]);
            Assertions.assertTrue(q.getLeftValues()[i] <= 10);
            Assertions.assertTrue(1 <= q.getRightValues()[i]);
            Assertions.assertTrue(q.getRightValues()[i] <= 10);
        }
    }


    @Test
    @DisplayName("Test2")
    void test2() {
        Random random = new Random();

        for (int i = 0; i < 2000; i++) {
            int a = random.nextInt(10);
            int b = 100 + random.nextInt(20);
            int c = 1 + random.nextInt(20);

            QuestionSetOption q = new QuestionSetOption(a, b, c);
            Assertions.assertEquals(q.min, a);
            Assertions.assertEquals(q.max, b);
            Assertions.assertEquals(q.N, c);
        }
    }

    @Test
    @DisplayName("Test3")
    void test3() {
        // Eğer dosya oluşturulamazsa constructor bir RuntimeException fırlatır
        // Bu gerçekleşmemeli çünkü dosya yoksa oluşturulmalı, varsa da okunmalı :)
        Assertions.assertDoesNotThrow(() -> new DataManager());
    }


    @Test
    @DisplayName("Test4")
    void test4() {
        // Bu fonksiyon içine verilen millisecond cinsinden sayıyı string'e düzgün dönüştürmeli
        Assertions.assertEquals("00:00:03:0", Utils.formatTime(3000));
        Assertions.assertEquals("00:00:50:450", Utils.formatTime(50450));
        Assertions.assertEquals("00:13:07:896", Utils.formatTime(787896));
        Assertions.assertEquals("01:15:34:123", Utils.formatTime(4534123));
    }




}
