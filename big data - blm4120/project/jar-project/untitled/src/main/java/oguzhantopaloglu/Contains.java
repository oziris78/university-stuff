package oguzhantopaloglu;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;

public class Contains {

    public static class MyMapper extends Mapper<LongWritable, Text, LongWritable, Text> {
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            String line = value.toString();
            String[] columns = line.split(",");

            if (columns.length == 6) {
                String id = columns[0];
                String title = columns[4];
                String body = columns[5];

                Configuration configuration = context.getConfiguration();
                String searchKey = configuration.get("searchKey");

                if (body.contains(searchKey)) {
                    context.write(new LongWritable(1), new Text(id + " , " + title));
                }
            }
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        conf.set("searchKey", args[2]);
        Job job = Job.getInstance(conf, "Itertools Checker");

        job.setJarByClass(Contains.class);

        job.setMapperClass(MyMapper.class);

        job.setOutputKeyClass(LongWritable.class);
        job.setOutputValueClass(Text.class);

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
