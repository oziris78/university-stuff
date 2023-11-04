package oguzhantopaloglu;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.io.IntWritable;
import java.io.IOException;


public class Max {

    public static class MyMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
        private IntWritable score = new IntWritable();

        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            String line = value.toString();
            String[] columns = line.split(",");

            if (columns.length == 6) {
                try {
                    int scoreValue = Integer.parseInt(columns[3]);
                    score.set(scoreValue);
                    context.write(new Text("maxScore"), score);
                } catch (NumberFormatException e) {}
            }
        }
    }

    public static class MyReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
        @Override
        public void reduce(Text key, Iterable<IntWritable> values, Context context)
                throws IOException, InterruptedException {
            int maxScore = Integer.MIN_VALUE;

            for (IntWritable value : values) {
                maxScore = Math.max(maxScore, value.get());
            }

            context.write(new Text("Max Score"), new IntWritable(maxScore));
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Max Score");

        job.setJarByClass(Max.class);
        job.setMapperClass(MyMapper.class);
        job.setReducerClass(MyReducer.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(IntWritable.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        /*
        long starting, ending;
        starting = System.currentTimeMillis();
        job.waitForCompletion(true);
        ending = System.currentTimeMillis();
        System.out.println("Elapsed time: " + (ending-starting)); // or show it in GUI.
        */
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}


