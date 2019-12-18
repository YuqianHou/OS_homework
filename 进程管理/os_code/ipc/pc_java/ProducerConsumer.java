public class ProducerConsumer{
	static final int N = 10;
	static int current_item = 0;
	static producer p = new producer();
	static consumer c = new consumer();
	static our_monitor mon = new our_monitor();
	public static void main (String args[]){
		p.start();
		c.start();
	}
	static class producer extends Thread{
		public void run(){
			int item;
			while(true){
				item = produce_item();
				mon.insert(item);
			}
		}	
		private int produce_item(){
			current_item++;
			waste_time();
			return current_item;
		}	
		private static void waste_time(){
			try {				
				sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	static class consumer extends Thread{
		public void run(){
			int item;
			while(true){
				item = mon.remove();
				consume_item(item);
			}
		}
		private void consume_item(int item){
			waste_time();
		}
		private static void waste_time(){
			try {
				sleep(2000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	static class our_monitor{
		private int buffer[] = new int[N];
		private int count = 0,lo = 0, hi = 0;
		public synchronized void insert(int val){
			if(count == N) {
				System.out.println("Buffer full, producer sleeping...");				
				go_to_sleep();
			}
			buffer[hi] = val;
			hi = (hi+1)%N;
			count = count + 1;
			System.out.println("Item " + current_item + " is produced.");
			System.out.print("BUFFER: ");
			for(int i=0;i<N;i++){
				System.out.print(buffer[i]+" ");
			}
			System.out.println();
			if (count == 1) {
				System.out.println("Waking up the consumer...");
				notify();
			}
		}
		public synchronized int remove() {
			int val;
			if (count == 0) {
				System.out.println("Buffer empty, consumer sleeping...");
				go_to_sleep();
			}
			val = buffer[lo];
			buffer[lo] = 0;
			lo = (lo+1)%N;
			count = count -1;
			System.out.println("Item " + val + " is consumed.");
			System.out.print("BUFFER: ");
			for(int i=0;i<N;i++){
				System.out.print(buffer[i]+" ");
			}
			System.out.println();
			if (count == N-1) {
				System.out.println("Waking up the producer...");
				notify();
			}
			return val;
		}
		private void go_to_sleep(){
			try{
				wait();
			} catch(InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
